
#include "game/draw.h"
#include "piecedata.h"
#include "primitives.h"
#include "sugar.h"
#include "types.h"
const i64 PLAYFIELD_X0 = 5;
const i64 PLAYFIELD_GXOFFSET = 5;
const i64 PLAYFIELD_Y0 = 0;
const i64 SIDE_WALL_WIDTH = 5;
const i64 PLAYFIELD_WIDTH = 10;
const i64 PLAYFIELD_HEIGHT = 20;
const i64 PLAYFIELD_X1 = PLAYFIELD_X0 + PLAYFIELD_WIDTH;
const i64 PLAYFIELD_Y1 = PLAYFIELD_Y0 + PLAYFIELD_HEIGHT;
const i64 PLAYFIELD_GYOFFSET = DEF_ROWS - PLAYFIELD_HEIGHT;

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

void drawPiece(vec2 g_originPos, PieceType T) {
    PieceDef* piece = get_piece_def(T);
    vec2      g_topLeftPos = vec2_sub(g_originPos, piece->l_origin);
    vec2      s_pos = vec2_mul(g_topLeftPos, BLOCK_SZ);
    drawTetro(s_pos, piece->offsets, piece->colors);
}

vec2 snapToGrid(const vec2 screenPos) {
    i64 ipx = round(screenPos.x) / BLOCK_SZ;
    i64 ipy = round(screenPos.y) / BLOCK_SZ;
    return (vec2){ ipx * BLOCK_SZ, ipy * BLOCK_SZ };
}

// draw tetro expects screen position in terms of the 'top left' of the 'sprite'
void drawTetro(vec2 s_pos, const vec2* offsets, const SDL_FColor* colors) {
#define OFFSET_LEN 4
    for (int i = 0; i < OFFSET_LEN; i++) {
        vec2 offset = offsets[i];
        drawBlockV((vec2){ s_pos.x + BLOCK_SZ * offset.x, s_pos.y + BLOCK_SZ * offset.y }, BLOCK_SZ,
                   colors);
    }
}
void drawBoundaries() {
    PieceDef* wall_piece = get_piece_def(PieceType_Wall_Piece);
    // draw floor
    {
        vec2 o = { 0, ctx.rows - PLAYFIELD_GYOFFSET };
        for (int x = 0; x < DEF_COLS; x++) {
            for (int y = 0; y < PLAYFIELD_GYOFFSET; y++) {
                vec2 pos = { o.x + x, o.y + y };
                pos = vec2_mul(pos, BLOCK_SZ);
                drawBlockV(pos, BLOCK_SZ, wall_piece->colors);
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
                drawBlockV(pos, BLOCK_SZ, wall_piece->colors);
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
                drawBlockV(pos, BLOCK_SZ, wall_piece->colors);
            }
        }
    }
}
void drawBlockV(vec2 screenPos, double len, const SDL_FColor piece_colors[4]) {
    double bord = len * 0.14;  // border width
    {
        // MAIN QUAD
        vec2 quad_verts[] = {
            screenPos,
            { screenPos.x + len, screenPos.y },
            { screenPos.x + len, screenPos.y + len },
            { screenPos.x, screenPos.y + len },
        };
        DrawQuad(quad_verts, piece_colors[2]);
    }
    {
        // LEFT EDGE
        vec2 o = { screenPos.x, screenPos.y };
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
        vec2 o = { screenPos.x + len - bord, screenPos.y };
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
        vec2 o = { screenPos.x, screenPos.y };
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
        vec2 o = { screenPos.x, screenPos.y + len };
        vec2 quad_verts[] = {
            { o.x, o.y },
            { o.x + bord, o.y - bord },
            { o.x + len - bord, o.y - bord },
            { o.x + len, o.y },
        };
        DrawQuad(quad_verts, piece_colors[0]);
    }
}

void drawBlockOutlineV(vec2 screenPos, double len, const SDL_FColor piece_colors[4]) {
    double bord = 1;  // border width
    {
        // LEFT EDGE
        vec2 o = { screenPos.x, screenPos.y };
        vec2 quad_verts[] = {
            o,
            { o.x + bord, o.y + bord },
            { o.x + bord, o.y + len - bord },
            { o.x, o.y + len },
        };
        DrawQuad(quad_verts, piece_colors[4]);
    }
    {
        // RIGHT EDGE
        vec2 o = { screenPos.x + len - bord, screenPos.y };
        vec2 quad_verts[] = {
            { o.x, o.y + bord },
            { o.x + bord, o.y },
            { o.x + bord, o.y + len },
            { o.x, o.y + len - bord },
        };
        DrawQuad(quad_verts, piece_colors[4]);
    }
    {
        // TOP EDGE
        vec2 o = { screenPos.x, screenPos.y };
        vec2 quad_verts[] = {
            { o.x, o.y },
            { o.x + len, o.y },
            { o.x + len - bord, o.y + bord },
            { o.x + bord, o.y + bord },
        };
        DrawQuad(quad_verts, piece_colors[4]);
    }
    {
        // BOT EDGE
        vec2 o = { screenPos.x, screenPos.y + len };
        vec2 quad_verts[] = {
            { o.x, o.y },
            { o.x + bord, o.y - bord },
            { o.x + len - bord, o.y - bord },
            { o.x + len, o.y },
        };
        DrawQuad(quad_verts, piece_colors[4]);
    }
}
void draw_block(double tlx, double tly, double len, const SDL_FColor piece_colors[4]) {
    drawBlockV((vec2){ tlx, tly }, len, piece_colors);
}

void drawGrid() {
    SDL_FColor previous_color;
    getcolor(previous_color);
    setcolor(gr(128));
    vec2 o = { PLAYFIELD_GXOFFSET, 0 };
    for (int y = 1; y < PLAYFIELD_HEIGHT; y++) {
        vec2 start = { o.x, o.y + y };
        vec2 end = { o.x + PLAYFIELD_WIDTH, o.y + y };
        start = vec2_mul(start, BLOCK_SZ);
        end = vec2_mul(end, BLOCK_SZ);
        DrawLine(start, end);
    }
    for (int x = 1; x < PLAYFIELD_WIDTH; x++) {
        vec2 start = { o.x + x, o.y };
        vec2 end = { o.x + x, o.y + PLAYFIELD_HEIGHT };
        start = vec2_mul(start, BLOCK_SZ);
        end = vec2_mul(end, BLOCK_SZ);
        DrawLine(start, end);
    }
    setcolor(previous_color);
}
