#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define DISABLE_WARNINGS

// #define DEBUG
#include "game/draw.h"
#include "game/piecequeue.h"

#include "PieceType.h"
#include "debug_overlay.h"
#include "gamecontext.h"
#include "gridcell.h"
#include "logger.h"
#include "piecedata.h"
#include "ringbuffer.h"
#include "sdlwrappers.h"
#include "sugar.h"
#include "timing.h"
#include "types.h"
#include "vec2.h"

static void overlay_setFields(void);
static void handleInputRequests(void);

static i64 row_flashDuration[(i64)DEF_ROWS] = {};
static i64 row_flashOpacity[(i64)DEF_ROWS] = {};  // 0-255
static i64 tick_last_frame;

GameContext gtx;
SDLContext  ctx;
static bool grid_isPieceSpaceFree(Piece p) {
    PieceData* pd = getPieceData(p.T);
    vec2       g_topLeftPos = p.g_pos;
    for (int blk_idx = 0; blk_idx < BLOCKS_PER_PIECE; blk_idx++) {
        i64      idx = getBlockOffset(gtx.activePiece.rotation, blk_idx);
        vec2     g_piecePos = local_to_grid(pd->l_blockOffsets[idx], g_topLeftPos);
        i64      gidx = get_grid_idxv(g_piecePos);
        GridCell cell = gtx.grid[gidx];
        if (gidx > DEF_CELLCOUNT || gidx <= 0) {
            return true;
        }
        if (cell.occupied) {
            LOG_ERROR("%.f,%.f is already occupied!", vec2_unpack(g_piecePos));
            return (false);
        }
    }
    return (true);
}

static bool grid_isPieceInBounds(Piece p) {
    PieceData* pd = getPieceData(p.T);
    vec2       g_topLeftPos = p.g_pos;
    for (int blk_idx = 0; blk_idx < BLOCKS_PER_PIECE; blk_idx++) {
        i64  idx = getBlockOffset(gtx.activePiece.rotation, blk_idx);
        vec2 g_piecePos = local_to_grid(pd->l_blockOffsets[idx], g_topLeftPos);
        if (g_piecePos.x >= PLAYFIELD_XMAX || g_piecePos.x < PLAYFIELD_XMIN ||
            g_piecePos.y >= PLAYFIELD_YMAX || g_piecePos.y < PLAYFIELD_YMIN) {
            // LOG_ERROR("%.f,%.f is out of bounds!", vec2_unpack(g_piecePos));
            return (false);
        }
    }
    return (true);
}

static bool grid_isPieceLocationValid(GridCell* grid, Piece p) {
    if (!gtx.isPieceActive)
        return (false);
    return grid_isPieceInBounds(p) && grid_isPieceSpaceFree(p);
}

static void grid_placeBlock(GridCell* grid, vec2 g_pos, ColorScheme colorscheme) {
    gtx.grid[get_grid_idxv(g_pos)] = (GridCell){ .colorscheme = colorscheme, .occupied = true };
    // LOGFN("%s", TOSTR(g_pos));
}

static void grid_placePiece(GridCell* grid, Piece activePiece) {
    if (!gtx.isPieceActive) {
        return;
    }

    PieceData* pd = getPieceData(activePiece.T);
    for (int i = 0; i < BLOCKS_PER_PIECE; i++) {
        int  idx = getBlockOffset(activePiece.rotation, i);
        vec2 g_pos = local_to_grid(pd->l_blockOffsets[idx], activePiece.g_pos);
        grid_placeBlock(grid, g_pos, pd->colorscheme);
    }
    gtx.isPieceActive = false;
    gtx.droptimer_current = gtx.droptimer;
}

static void game_start(SDL_Window* win, SDL_Renderer* rend, i64 w, i64 h) {
    tick_last_frame = 0;
    ctx = init_ctx(win, rend, w, h);
    gtx = init_GameContext();
}

static void game_restart(void) {
    game_start(ctx.window, ctx.renderer, ctx.w, ctx.h);
}

void profileFrameStats(i64* res_param_dt_ms) {
    ctx.frame_count++;  // i like the first frame to be 1
    //
    ctx.perf.ms_thisframe = get_current_ms(ctx.clock_freq);
    double dt = ctx.perf.ms_thisframe - ctx.perf.ms_lastframe;
    if (!ctx.perf.ms_lastframe) {
        dt = 1;
    }
    double fps = 1000.0 / dt;
    rb_push(ctx.perf.ft_rb, &dt);
    rb_push(ctx.perf.fps_rb, &fps);
    overlay_setFields();

    *res_param_dt_ms = (i64)dt;
}

void drawPieceQueue(PieceQueue pq) {
    PieceQueueNode* cur = pq.head;
    vec2            g_queue_pos = { ctx.cols - PLAYFIELD_XMIN + 1, 2 };
    for (int i = 0; i < pq.size; i++) {
        g_drawPiece(g_queue_pos, cur->type, 0, 255);
        g_queue_pos.y += 4;
        cur = cur->prev;  // moving down the queue towards the back
    }
}

static inline i64 signed_mod(i64 x, i64 mod) {
    return llabs((x % mod));
}

void piece_rotateLeft(Piece* p) {
    p->rotation = signed_mod(p->rotation - 1, Direction_COUNT);
}
void piece_rotateRight(Piece* p) {
    p->rotation = signed_mod(p->rotation + 1, Direction_COUNT);
}
void attemptPieceRotation(Direction dir) {
    const Direction rotationBefore = gtx.activePiece.rotation;
    switch (dir) {
    case Direction_LEFT:
        piece_rotateLeft(&gtx.activePiece);
        if (!grid_isPieceLocationValid(gtx.grid, gtx.activePiece)) {
            gtx.activePiece.rotation = rotationBefore;
        }
        break;

    case Direction_RIGHT:
        piece_rotateRight(&gtx.activePiece);
        if (!grid_isPieceLocationValid(gtx.grid, gtx.activePiece)) {
            gtx.activePiece.rotation = rotationBefore;
        }
        break;
    default:
        LOG_FATAL("Rotation of %s is unsupported.", Direction_tostr(dir));
        break;
    }
}

static bool attemptPieceMove(Direction dir) {
    bool moved = false;
    switch (dir) {
    case Direction_UP:
        LOG_FATAL("Bro defies gravity");
        break;
    case Direction_DOWN:
        gtx.activePiece.g_pos.y++;
        if (!grid_isPieceLocationValid(gtx.grid, gtx.activePiece)) {
            gtx.activePiece.g_pos.y--;
            grid_placePiece(gtx.grid, gtx.activePiece);
        } else {
            moved = true;
        }
        break;

    case Direction_LEFT:
        gtx.activePiece.g_pos.x--;
        if (!grid_isPieceLocationValid(gtx.grid, gtx.activePiece)) {
            gtx.activePiece.g_pos.x++;
        } else {
            moved = true;
        }
        break;

    case Direction_RIGHT:
        gtx.activePiece.g_pos.x++;
        if (!grid_isPieceLocationValid(gtx.grid, gtx.activePiece)) {
            gtx.activePiece.g_pos.x--;
        } else {
            moved = true;
        }

        break;
    default:
        LOG_FATAL("Unknown direction (%d) supplied.", dir);
        break;
    }

    return moved;
}

static void app_clear(SDL_FColor color) {
    setcolor(color);
    SDL_RenderClear(ctx.renderer);
}

static void app_draw(void) {
    app_clear(ctx.draw.clear_color);
    drawBackground(row_flashOpacity);
    if (gtx.isPieceActive) {
        g_drawPiece(gtx.activePiece.g_pos, gtx.activePiece.T, gtx.activePiece.rotation, 255);
    }
    drawDebugOverlay(true);
    drawPlayfield(row_flashOpacity);
    drawPieceQueue(gtx.piecequeue);
    if (gtx.state == GameStateGameOver) {
        // draw game over screen
    }

    SDL_RenderPresent(ctx.renderer);
}
Piece piecequeue_getNewPiece(PieceQueue* piecequeue, const Piece* old) {
    PieceType T = pq_pop(piecequeue);
    pq_push(piecequeue, random_piece_type());
    Piece res = (Piece){
        .T = T,
        .g_pos = get_initial_position(old->T),
        .rotation = get_initial_rotation(old->T),
    };
    return res;
}

static bool grid_isRowFull(GridCell* grid, i64 row) {
    for (int col = PLAYFIELD_XMIN; col < PLAYFIELD_XMAX; col++) {
        int idx = get_grid_idx(col, row);
        if (!grid[idx].occupied) {
            return false;
        }
    }
    return true;
}

static bool grid_isRowEmpty(GridCell* grid, i64 row) {
    for (int col = PLAYFIELD_XMIN; col < PLAYFIELD_XMAX; col++) {
        int idx = get_grid_idx(col, row);
        if (grid[idx].occupied) {
            return false;
        }
    }
    return true;
}

static void grid_swapRows(GridCell* grid, i64 rowA, i64 rowB) {
    for (int col = PLAYFIELD_XMIN; col < PLAYFIELD_XMAX; col++) {
        int idxA = get_grid_idx(col, rowA);
        int idxB = get_grid_idx(col, rowB);

        GridCell temp = grid[idxA];
        grid[idxA] = grid[idxB];
        grid[idxB] = temp;
    }
}
static void grid_bubbleRowsDown(GridCell* grid, i64 startingRow) {
    for (int row = startingRow; row > PLAYFIELD_YMIN; row -= 1) {
        grid_swapRows(grid, row, row - 1);
    }
}

const i64   ticks_flashDuration = 40;
static void row_enableFlash(GridCell* grid, i64 row) {
    row_flashDuration[row] = ticks_flashDuration;
}

static void grid_clearRow(GridCell* grid, i64 row) {
    for (int col = PLAYFIELD_XMIN; col < PLAYFIELD_XMAX; col++) {
        int idx = get_grid_idx(col, row);
        grid[idx].occupied = false;
    }
    LOG_EXPR(row);
    row_enableFlash(grid, row);
}
float modulate(i64 tick, i64 flashduration) {
    float       t = tick;
    const float sx = 5;
    const float ymin = 0;
    const float ymax = (128 / ticks_flashDuration) * flashduration;
    //        const float ymax = (128);
    const float period = 10;
    const float mid = (ymin + ymax) / 2.0;
    const float height = ymax - ymin;

    float a = height / 2.0;
    float b = period;
    float c = -sx * (M_PI / 10.0);
    float d = mid;
    return (sin(t * b + c) * a) + d;
}
static void grid_update(GridCell* grid, i64 deltaTicks, i64 tick) {
    for (int row = PLAYFIELD_YMAX; row >= 0; row--) {
        if (grid_isRowFull(grid, row)) {
            grid_clearRow(grid, row);
        }
        if (row_flashDuration[row] > 0) {
            row_flashDuration[row] -= deltaTicks;
        }
        row_flashOpacity[row] = modulate(tick, row_flashDuration[row]);
    }
    for (int row = PLAYFIELD_YMAX; row >= 0; row--) {
        if (grid_isRowEmpty(grid, row)) {
            grid_bubbleRowsDown(grid, row);
        }
    }
}
static void app_update(i64 dt_ms, GameContext* gtx) {
    const int app_TPS = 20;

    if (gtx->state == GameStateGameOver) {
        // LOG_ERROR("Game over!");
        return;
    }
    gtx->tick = (i64)(ms_since_start() / (1000.0 / app_TPS));  // 20 tps
    i64 deltaTick = gtx->tick - tick_last_frame;

    if (deltaTick > 0) {
        if (gtx->isPieceActive) {
            if (gtx->tick % gtx->moveDownDelay == 0) {
                attemptPieceMove(Direction_DOWN);
            }
            handleInputRequests();
        } else {  // piece is not active, decrement by dtick
            gtx->droptimer_current -= deltaTick;
        }
    }

    grid_update(gtx->grid, deltaTick, gtx->tick);
    if (!gtx->isPieceActive && gtx->droptimer_current <= 0) {
        gtx->activePiece = piecequeue_getNewPiece(&gtx->piecequeue, &gtx->activePiece);
        gtx->isPieceActive = true;
        // LOG_EXPR(gtx->activePiece);
        // LOG_NOTICE("Spawned new piece @%.f,%.f", vec2_unpack(gtx->activePiece.g_pos));
        if (!grid_isPieceSpaceFree(gtx->activePiece)) {
            LOG_NOTICE("Game over!");
            gtx->state = GameStateGameOver;
            return;
        }
    }
    tick_last_frame = gtx->tick;
}

typedef enum { Event_Continue, Event_Exit } EventResult;
EventResult app_handleEvent(InputRequests* requests, SDL_Event* event) {
    switch (event->type) {
    case SDL_EVENT_KEY_DOWN:
        switch (event->key.key) {
        case SDLK_DOWN:
            [[fallthrough]];
        case SDLK_S:
            requests->fast_drop_pressed = true;
            break;

        case SDLK_UP:
            [[fallthrough]];
        case SDLK_W:
            requests->instant_drop_pressed = true;
            break;

        case SDLK_A:
            requests->rotate_left = true;
            break;

        case SDLK_D:
            requests->rotate_right = true;
            break;

        case SDLK_H:  // vim keybinds
            [[fallthrough]];
        case SDLK_LEFT:
            requests->move_left_pressed = true;
            break;

        case SDLK_L:  // vim keybinds
            [[fallthrough]];
        case SDLK_RIGHT:
            requests->move_right_pressed = true;
            break;

        case SDLK_R:
            game_restart();
            break;
        }
        break;

    case SDL_EVENT_QUIT:
        return Event_Exit;
        break;

    default:
        return Event_Continue;
        break;
    }
    return Event_Continue;
}

static void handleInputRequests(void) {
#define handle(req, action)                                                                        \
    if (ctx.inputReq.req) {                                                                        \
        action;                                                                                    \
        ctx.inputReq.req = false;                                                                  \
    }

    handle(rotate_left, attemptPieceRotation(Direction_LEFT));
    handle(rotate_right, attemptPieceRotation(Direction_RIGHT));
    handle(rotate_right, attemptPieceRotation(Direction_RIGHT));
    handle(move_left_pressed, attemptPieceMove(Direction_LEFT));
    handle(move_right_pressed, attemptPieceMove(Direction_RIGHT));
    handle(fast_drop_pressed, attemptPieceMove(Direction_DOWN));
    handle(instant_drop_pressed, while (attemptPieceMove(Direction_DOWN)){});
}

//
/* This function runs once at shutdown. */

void overlay_setFields(void) {
    overlay_reset();
    [[maybe_unused]] vec2        mpos = { ctx.inputReq.s_mpos.x, ctx.inputReq.s_mpos.y };
    [[maybe_unused]] vec2        s_mpos = screen_to_grid(mpos);
    [[maybe_unused]] const char* m1str = (ctx.inputReq.m1_pressed ? "DOWN" : "UP");
    OVERLAY_PRINTLN("mpos: %.4f, %.4f", mpos.x, mpos.y);
    //   OVERLAY_PRINTLN("m1: %s", m1str);
    //   OVERLAY_PRINTLN("spos: %.2f %.2f", vec2_unpack(ctx.input.s_mpos));
    //    OVERLAY_PRINTLN("gpos: %.2f %.2f", vec2_unpack(s_mpos));
    OVERLAY_PRINTLN("tick: %lld", gtx.tick);
    OVERLAY_PRINTLN("rotation: %s", Direction_tostr(gtx.activePiece.rotation));
    OVERLAY_PRINTLN("gpos: %.0f, %.0f", vec2_unpack(gtx.activePiece.g_pos));
    OVERLAY_PRINTLN("active: %d", gtx.isPieceActive);
    OVERLAY_PRINTLN("droptimer: %lld", gtx.droptimer_current);
    OVERLAY_PRINTLN("state: %lld", gtx.state);

    if (ctx.perf.show_perf_in_debug) {
        OVERLAY_PRINTLN("frametime: %.4lf", dbl_rb_avg(ctx.perf.ft_rb));
        // OVERLAY_PRINTLN("framecount: %ld", ctx.frame_count);
        OVERLAY_PRINTLN("fps: %.1lf", dbl_rb_avg(ctx.perf.fps_rb));
    }
}

int app_init(int argc, char* argv[]) {
    SDL_SetAppMetadata("cstris", "1.0", "lmeldrum.cstris");
    SDL_Window*   window = NULL;
    SDL_Renderer* renderer = NULL;
    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("cstris - window", DEF_WIDTH, DEF_HEIGHT,
                                     SDL_WINDOW_TRANSPARENT, &window, &renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(renderer, DEF_WIDTH, DEF_HEIGHT,
                                     SDL_LOGICAL_PRESENTATION_LETTERBOX);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

    game_start(window, renderer, DEF_WIDTH, DEF_HEIGHT);
    //    SDL_SetRenderVSync(ctx.renderer, 1);
    return SDL_APP_CONTINUE; /* carry on with the program! */
}

int app_exit(int exitCode) {
    LOG_INFO("Exiting. (%d),", exitCode);
    // we must now deinitialize everyything manually i think
    return exitCode;
}

int main(int argc, char** argv) {
    app_init(argc, argv);

    SDL_Event event = {};
    i64       dt_ms = {};

    while (ctx.running) {
        profileFrameStats(&dt_ms);
        while (SDL_PollEvent(&event)) {
            if (app_handleEvent(&ctx.inputReq, &event) == Event_Exit) {
                ctx.running = false;
                goto EXIT_LOOP;
            }
        }

        app_update(dt_ms, &gtx);
        app_draw();

        ctx.perf.ms_lastframe = ctx.perf.ms_thisframe;
    }

EXIT_LOOP:
    return app_exit(EXIT_SUCCESS);
}
