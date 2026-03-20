#include "game/draw.h"
#include "game/internal_draw.h"  // src/game/internal_draw.c
#include "logger.h"
#include "piecedata.h"
#include "primitives.h"
#include "sugar.h"
#include "timing.h"
#include "types.h"
#include <math.h>

void g_drawPieceDebugMode(const vec2 g_topLeftPos, const vec2* l_blockOffsets, PieceType T);

void g_drawPiece(vec2 g_topLeftPos, PieceType T) {
    PieceData* piece = get_piece_data(T);
    vec2       s_pos = grid_to_screen(g_topLeftPos);

    // TESTING ROTATION, ACTUAL ROTATION WILL APPLY TO OFFSETS
    //    vec2 rotated_offsets[OFFSET_LEN] = {};
    //    for (int i = 0; i < OFFSET_LEN; i++) {
    //        rotated_offsets[i] = piece->l_blockOffsets[i];
    //        double rotation = M_PI_2 * 1;  // ctx.rotation_idx;
    //        rotated_offsets[i] = rotate_vec2(rotated_offsets[i], rotation);
    //    }
    // TESTING ROTATION, ACTUAL ROTATION WILL APPLY TO OFFSETS

    for (int i = 0; i < OFFSET_LEN; i++) {
        vec2 g_blockPos = local_to_grid(piece->l_blockOffsets[i], g_topLeftPos);
        vec2 s_blockPos = grid_to_screen(g_blockPos);
        LOGLN("%d: offset: L={%.2f,%.2f},G={%.2f,%.2f} S={%.2f,%.2f}", i,
              vec2_unpack(piece->l_blockOffsets[i]), vec2_unpack(g_blockPos),
              vec2_unpack(s_blockPos));
        s_drawBlock(s_blockPos, BLOCK_SZ, piece->colorscheme);
    }
    g_drawPieceDebugMode(g_topLeftPos, piece->l_blockOffsets, piece->type);
}
void g_drawPieceDebugMode(const vec2 g_topLeftPos, const vec2* l_blockOffsets, PieceType T) {
    // find the min and max local coords in offsets
    vec2 l_min = {};
    vec2 l_max = {};
    for (int i = 0; i < OFFSET_LEN; i++) {
        l_min = vec2_min(l_min, l_blockOffsets[i]);
        l_max = vec2_max(l_max, l_blockOffsets[i]);
    }

    // we want the 'outline'
    {
        vec2 _one = { 1, 1 };
        l_min = vec2_sub(l_min, _one);
        l_max = vec2_add(l_max, _one);
    }

    vec2 g_min = local_to_grid(l_min, g_topLeftPos);
    vec2 g_max = local_to_grid(l_max, g_topLeftPos);

    PieceData* pd = get_piece_data(T);
    vec2       g_rot_origin = vec2_add(g_topLeftPos, pd->l_rot_origin);
    //    drawGrid(g_min, g_max);
    // draw the bounds (min and max)
    g_drawBlock(g_max, BLOCK_SZ, grey);
    g_drawBlock(g_min, BLOCK_SZ, grey);

    // draw the origin
    g_drawBlockSpecial(g_rot_origin, BLOCK_SZ, WHITE);
}
void s_drawPieceSpecial(vec2 s_pos, const vec2* offsets, const ColorScheme colorscheme) {
    for (int i = 0; i < OFFSET_LEN; i++) {
        vec2 offset = offsets[i];
        s_drawBlockSpecial((vec2){ s_pos.x + BLOCK_SZ * offset.x, s_pos.y + BLOCK_SZ * offset.y },
                           BLOCK_SZ, colorscheme);
    }
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

void drawWalls(void) {
    PieceData* wall_piece = get_piece_data(PieceType_Wall_Piece);
    // draw floor
    {
        vec2 o = { 0, ctx.rows - PLAYFIELD_GYOFFSET };
        for (int x = 0; x < DEF_COLS; x++) {
            for (int y = 0; y < PLAYFIELD_GYOFFSET; y++) {
                vec2 pos = { o.x + x, o.y + y };
                pos = vec2_mul(pos, BLOCK_SZ);
                s_drawBlock(pos, BLOCK_SZ, wall_piece->colorscheme);
            }
        }
    }
    {
        // draw left wall
        vec2 g_leftWall = { 0, 0 };
        vec2 g_rightWall = { ctx.cols - PLAYFIELD_GXOFFSET, 0 };
        vec2 s_leftWall = grid_to_screen(g_leftWall);
        vec2 s_rightWall = grid_to_screen(g_rightWall);
        for (int x = 0; x < PLAYFIELD_GXOFFSET; x += BLOCK_SZ) {
            for (int y = 0; y < DEF_ROWS; y += BLOCK_SZ) {
                vec2 s_leftWall = { s_leftWall.x + x, s_leftWall.y + y };
                vec2 s_rightWall = { PLAYFIELD_WIDTH + s_rightWall.x + x, s_rightWall.y + y };
                s_drawBlock(s_leftWall, BLOCK_SZ, wall_piece->colorscheme);
                s_drawBlock(s_rightWall, BLOCK_SZ, wall_piece->colorscheme);
            }
        }
    }
}
