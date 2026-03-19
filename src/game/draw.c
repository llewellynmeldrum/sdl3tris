#include "game/draw.h"
#include "logger.h"
#include "piecedata.h"
#include "primitives.h"
#include "sugar.h"
#include "timing.h"
#include "types.h"
#include <math.h>

// thinking about rotation, i observed that tetros seem to have an 'origin' point.
// This is a point within their own grid about which they rotate.
// For example, a I block rotates like this:
// |0█00|    |0000|
// |0▒00| -> |█▒██|
// |0█00|    |0000|
// |0█00|    |0000|
// The origin point' is highlighted as '▒'
// It might be wise to refer to tetrominoes/pieces by their origin point.
// Thus the draw commands should probably take this origin?
static inline void g_drawBlock(vec2 gridPos, double len, const SDL_FColor piece_colors[5]);
static inline void drawTetro(vec2 s_pos, const vec2* offsets, const SDL_FColor* colors);
static inline void drawTetroSpecial(vec2 s_pos, const vec2* offsets, const SDL_FColor* colors);
static inline void s_drawBlockSpecial(vec2 s_pos, double len, const SDL_FColor piece_colors[5]);
static inline void g_drawBlockSpecial(vec2 g_topLeftPos, double extent,
                                      const SDL_FColor piece_colors[4]);

// #define DRAW_BOUNDING_BOX
void drawTetroBounds(PieceType T, const vec2* offsets, const vec2 g_topLeftPos) {
#define OFFSET_LEN 4
    // find the min and max local coords in offsets
    vec2 l_min = {};
    vec2 l_max = {};
    for (int i = 0; i < OFFSET_LEN; i++) {
        l_min = vec2_min(l_min, offsets[i]);
        l_max = vec2_max(l_max, offsets[i]);
    }

    // we want the 'outline'
    {
        vec2 _one = { 1, 1 };
        l_min = vec2_sub(l_min, _one);
        l_max = vec2_add(l_max, _one);
    }

    vec2 g_min = local_to_grid(l_min, g_topLeftPos);
    vec2 g_max = local_to_grid(l_max, g_topLeftPos);

    PieceData* pd = get_piece_def(T);
    vec2       g_rot_origin = vec2_add(g_topLeftPos, pd->l_rot_origin);
    //    drawGrid(g_min, g_max);
    // draw the bounds
    g_drawBlock(g_max, BLOCK_SZ, grey);
    g_drawBlock(g_min, BLOCK_SZ, grey);

    // draw the origin
    g_drawBlockSpecial(g_rot_origin, BLOCK_SZ, WHITE);
}
static inline void g_drawBlockSpecial(vec2 g_topLeftPos, double extent,
                                      const SDL_FColor piece_colors[4]) {
    vec2 s_topLeftPos = grid_to_screen(g_topLeftPos);
    s_drawBlockSpecial(s_topLeftPos, extent, piece_colors);
}
void s_drawBlockSpecial(vec2 s_pos, double len, const SDL_FColor piece_colors[5]) {
    SDL_FColor translucent_colors[5] = {};
    for (int i = 0; i < 5; i++) {
        translucent_colors[i] = piece_colors[i];
        translucent_colors[i].a = 0.5;
    }
    s_drawBlock(s_pos, len, translucent_colors);
    float      th = len * 0.14;
    SDL_FColor black = (SDL_FColor){ 0.1, 0.1, 0.1, 0.5 };
    {
        // TL->BR LINE
        vec2 tl = { s_pos.x, s_pos.y };
        vec2 br = { s_pos.x + len, s_pos.y + len };
        vec2 quad_verts[] = {
            { tl.x, tl.y + th },
            { tl.x + th, tl.y },
            { br.x, br.y - th },
            { br.x - th, br.y },
        };
        DrawQuad(quad_verts, black);
    }
    {
        // TL->BR LINE
        vec2 tr = { s_pos.x + len, s_pos.y };
        vec2 bl = { s_pos.x, s_pos.y + len };
        vec2 quad_verts[] = {
            { tr.x - th, tr.y },
            { tr.x, tr.y + th },
            { bl.x + th, bl.y },
            { bl.x, bl.y - th },
        };
        DrawQuad(quad_verts, black);
    }
}
void drawTetroSpecial(vec2 s_pos, const vec2* offsets, const SDL_FColor* colors) {
#define OFFSET_LEN 4
    for (int i = 0; i < OFFSET_LEN; i++) {
        vec2 offset = offsets[i];
        s_drawBlockSpecial((vec2){ s_pos.x + BLOCK_SZ * offset.x, s_pos.y + BLOCK_SZ * offset.y },
                           BLOCK_SZ, colors);
    }
}
void drawPiece(vec2 g_topLeftPos, PieceType T) {
    PieceData* piece = get_piece_def(T);
    vec2       s_pos = vec2_mul(g_topLeftPos, BLOCK_SZ);

    vec2 offsets[OFFSET_LEN] = {};
    for (int i = 0; i < OFFSET_LEN; i++) {
        offsets[i] = piece->offsets[i];
        int    rotation_idx = ((int)get_current_ms() / 1000) % 4;
        double rotation = M_PI_2 * rotation_idx;
        offsets[i] = rotate_vec2(offsets[i], rotation);
    }

    drawTetro(s_pos, offsets, piece->colors);
    drawTetroBounds(T, piece->offsets, g_topLeftPos);
}

vec2 snapToGrid(const vec2 s_pos) {
    i64 ipx = round(s_pos.x) / BLOCK_SZ;
    i64 ipy = round(s_pos.y) / BLOCK_SZ;
    return (vec2){ ipx * BLOCK_SZ, ipy * BLOCK_SZ };
}

// draw tetro expects screen position in terms of the 'top left' of the 'sprite'
void drawTetro(vec2 s_pos, const vec2* offsets, const SDL_FColor* colors) {
#define OFFSET_LEN 4
    for (int i = 0; i < OFFSET_LEN; i++) {
        vec2 offset = offsets[i];
        s_drawBlock((vec2){ s_pos.x + BLOCK_SZ * offset.x, s_pos.y + BLOCK_SZ * offset.y },
                    BLOCK_SZ, colors);
    }
}

void s_drawBlock(vec2 s_pos, double len, const SDL_FColor piece_colors[4]) {
    double bord = len * 0.14;  // border width
    {
        // MAIN QUAD
        vec2 quad_verts[] = {
            s_pos,
            { s_pos.x + len, s_pos.y },
            { s_pos.x + len, s_pos.y + len },
            { s_pos.x, s_pos.y + len },
        };
        DrawQuad(quad_verts, piece_colors[2]);
    }
    {
        // LEFT EDGE
        vec2 o = { s_pos.x, s_pos.y };
        vec2 quad_verts[] = {
            o,
            { o.x + bord, o.y + bord },
            { o.x + bord, o.y + len - bord },
            { o.x, o.y + len },
        };
        DrawQuad(quad_verts, piece_colors[1]);
    }
    {
        // RIGHT EDGE
        vec2 o = { s_pos.x + len - bord, s_pos.y };
        vec2 quad_verts[] = {
            { o.x, o.y + bord },
            { o.x + bord, o.y },
            { o.x + bord, o.y + len },
            { o.x, o.y + len - bord },
        };
        DrawQuad(quad_verts, piece_colors[1]);
    }
    {
        // TOP EDGE
        vec2 o = { s_pos.x, s_pos.y };
        vec2 quad_verts[] = {
            { o.x, o.y },
            { o.x + len, o.y },
            { o.x + len - bord, o.y + bord },
            { o.x + bord, o.y + bord },
        };
        DrawQuad(quad_verts, piece_colors[3]);
    }
    {
        // BOT EDGE
        vec2 o = { s_pos.x, s_pos.y + len };
        vec2 quad_verts[] = {
            { o.x, o.y },
            { o.x + bord, o.y - bord },
            { o.x + len - bord, o.y - bord },
            { o.x + len, o.y },
        };
        DrawQuad(quad_verts, piece_colors[0]);
    }
}

static inline void g_drawBlock(vec2 g_topLeftPos, double extent, const SDL_FColor piece_colors[5]) {
    vec2 s_topLeftPos = grid_to_screen(g_topLeftPos);
    s_drawBlock(s_topLeftPos, extent, piece_colors);
}
void drawGrid(vec2 g_min, vec2 g_max) {
    u64 width = g_max.x - g_min.x;
    u64 height = g_max.y - g_min.y;

    LOG("%llu\n", height);
    LOG("%lf,%lf, %lf,%lf\n", vec2_unpack(g_min), vec2_unpack(g_max));
    LOG("%llu\n", width);
    SDL_FColor previous_color;
    getcolor(previous_color);
    setcolor(GREY(128));

    {
        // horizontal lines
        vec2 start = { g_min.x, g_min.y };
        vec2 end = { g_min.x + width, g_min.y };
        for (int y = 1; y < height; y++) {
            start.y += 1;
            end.y += 1;
            vec2 s_start = vec2_mul(start, BLOCK_SZ);
            vec2 s_end = vec2_mul(end, BLOCK_SZ);
            DrawLine(s_start, s_end);
        }
    }
    {
        // vertical lines
        vec2 start = { g_min.x, g_min.y };
        vec2 end = { g_min.x, g_min.y + height };
        for (int x = 1; x < width; x++) {
            start.x += 1;
            end.x += 1;
            vec2 s_start = vec2_mul(start, BLOCK_SZ);
            vec2 s_end = vec2_mul(end, BLOCK_SZ);
            DrawLine(s_start, s_end);
        }
    }
    setcolor(previous_color);
}
void drawWalls() {
    return;
    PieceData* wall_piece = get_piece_def(PieceType_Wall_Piece);
    // draw floor
    {
        vec2 o = { 0, ctx.rows - PLAYFIELD_GYOFFSET };
        for (int x = 0; x < DEF_COLS; x++) {
            for (int y = 0; y < PLAYFIELD_GYOFFSET; y++) {
                vec2 pos = { o.x + x, o.y + y };
                pos = vec2_mul(pos, BLOCK_SZ);
                s_drawBlock(pos, BLOCK_SZ, wall_piece->colors);
            }
        }
    }
    {
        // draw left wall
        vec2 o = { 0, 0 };
        for (int x = 0; x < PLAYFIELD_GXOFFSET; x++) {
            for (int y = 0; y < DEF_ROWS; y++) {
                vec2 pos = { o.x + x, o.y + y };
                pos = vec2_mul(pos, BLOCK_SZ);
                s_drawBlock(pos, BLOCK_SZ, wall_piece->colors);
            }
        }
    }
    {
        // draw right wall
        vec2 o = { ctx.cols - PLAYFIELD_GXOFFSET, 0 };
        for (int x = 0; x < PLAYFIELD_GXOFFSET; x++) {
            for (int y = 0; y < DEF_ROWS; y++) {
                vec2 pos = { o.x + x, o.y + y };
                pos = vec2_mul(pos, BLOCK_SZ);
                s_drawBlock(pos, BLOCK_SZ, wall_piece->colors);
            }
        }
    }
}
