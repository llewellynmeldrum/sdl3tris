#include "sugar.h"
#include <math.h>
#include <stdlib.h>
#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "debugtext.h"
#include "game/draw.h"
#include "piecedata.h"
#include "ringbuffer.h"
#include "sdlwrappers.h"
#include "sugar.h"
#include "timing.h"
#include "types.h"

SDL_AppResult SDL_AppIterate(void* _) {
    ctx.frame_count++;  // i like the first frame to be 1
    double ms_thisframe = get_current_ms();
    double ft_ms = ms_thisframe - ctx.perf.ms_lastframe;
    double fps = 1000.0 / (ft_ms);
    rb_push(ctx.perf.ft_rb, &ft_ms);
    rb_push(ctx.perf.fps_rb, &fps);

    setcolor(ctx.draw.clear_color);
    SDL_RenderClear(ctx.renderer);

    drawBoundaries();
    drawGrid();

    drawPiece(screen_to_grid(ctx.input.mpos), PieceType_I_Piece);
    //    vec2 i_piece_offsets[] = {
    //        { 0, 0 },
    //        { 0, 1 },
    //        { 0, 2 },
    //        { 0, 3 },
    //    };
    //    drawTetro(ctx.input.mgpos, i_piece_offsets);
    //
    /*

    o.x += ext * 3;
    vec2 j_piece_offsets[] = {
            {0, 0},
            {0, 1},
            {0, 2},
            {-1, 2},
    };
    for (int i = 0; i < ARRLEN(j_piece_offsets); i++) {
        vec2 offset = j_piece_offsets[i];
        drawBlockV((vec2){o.x + ext * offset.x, o.y + ext * offset.y}, ext, blue);
    }

    {
        o.x += ext * 3;
        vec2 l_piece_offsets[] = {
                {0, 0},
                {0, 1},
                {0, 2},
                {1, 2},
        };
        for (int i = 0; i < ARRLEN(l_piece_offsets); i++) {
            vec2 offset = l_piece_offsets[i];
            drawBlockV((vec2){o.x + ext * offset.x, o.y + ext * offset.y}, ext, orange);
        }
    }
    {
        o.x += ext * 3;
        vec2 o_piece_offsets[] = {
                {0, 0},
                {0, 1},
                {1, 0},
                {1, 1},
        };
        for (int i = 0; i < ARRLEN(o_piece_offsets); i++) {
            vec2 offset = o_piece_offsets[i];
            drawBlockV((vec2){o.x + ext * offset.x, o.y + ext * offset.y}, ext, yellow);
        }
    }
    {
        o.x = 50;
        o.y += ext * 5;
        vec2 s_piece_offsets[] = {
                {0, 1},
                {1, 1},
                {1, 0},
                {2, 0},
        };
        for (int i = 0; i < ARRLEN(s_piece_offsets); i++) {
            vec2 offset = s_piece_offsets[i];
            drawBlockV((vec2){o.x + ext * offset.x, o.y + ext * offset.y}, ext, green);
        }
    }
    {
        o.x += ext * 4;
        vec2 t_piece_offsets[] = {
                {0, 0},
                {1, 0},
                {2, 0},
                {1, 1},
        };
        for (int i = 0; i < ARRLEN(t_piece_offsets); i++) {
            vec2 offset = t_piece_offsets[i];
            drawBlockV((vec2){o.x + ext * offset.x, o.y + ext * offset.y}, ext, purple);
        }
    }
    {
        o.x += ext * 5;
        vec2 s_piece_offsets[] = {
                {0, 0},
                {1, 0},
                {1, 1},
                {2, 1},
        };
        for (int i = 0; i < ARRLEN(s_piece_offsets); i++) {
            vec2 offset = s_piece_offsets[i];
            drawBlockV((vec2){o.x + ext * offset.x, o.y + ext * offset.y}, ext, red);
        }
    }

    */
    drawDebugWindow();

    SDL_RenderPresent(ctx.renderer);
    ctx.perf.ms_lastframe = ms_thisframe;
    ctx.perf.show_perf_in_debug = false;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppInit(void** _, int argc, char* argv[]) {
    ctx = init_ctx();
    ctx.perf.ms_lastframe = get_current_ms();
    SDL_SetAppMetadata("cstris", "1.0", "lmeldrum.cstris");

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("cstris - window", ctx.w, ctx.h, 0, &ctx.window,
                                     &ctx.renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(ctx.renderer, ctx.w, ctx.h,
                                     SDL_LOGICAL_PRESENTATION_LETTERBOX);

    ctx.clock_freq = SDL_GetPerformanceFrequency();
    return SDL_APP_CONTINUE; /* carry on with the program! */
}

void printmpos() {
    double x = ctx.input.mpos.x;
    double y = ctx.input.mpos.y;
    printf("CLICK @: %.2f, %.2f\n", x, y);
}
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    (void)appstate;
    switch (event->type) {
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        ctx.input.m1down = (event->button.button == 1) ? true : ctx.input.m1down, printmpos();
        ctx.input.m2down = (event->button.button == 2) ? true : ctx.input.m2down;
        break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
        ctx.input.m1down = (event->button.button == 1) ? false : ctx.input.m1down;
        ctx.input.m2down = (event->button.button == 2) ? false : ctx.input.m2down;
        break;

    case SDL_EVENT_MOUSE_MOTION:
        ctx.input.mpos = (vec2){ event->motion.x, event->motion.y };
        ctx.input.mgpos = snapToGrid((vec2){ round(event->motion.x), round(event->motion.y) });
        break;

    case SDL_EVENT_QUIT:  //
        return SDL_APP_SUCCESS;
        break;

    default:  //
        return SDL_APP_CONTINUE;
        break;
    }
    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    (void)appstate;
    (void)result;
    /* SDL will clean up the window/renderer for us. */
}
