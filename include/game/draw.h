#pragma once
#include "piecedata.h"
#include "sdlwrappers.h"
#include "sugar.h"

// src/game/draw.c
void drawPieceOrigin(vec2 g_originPos, PieceType T);
void drawPiece(vec2 g_topLeftPos, PieceType T);

void s_drawBlock(vec2 screenPos, double len, const SDL_FColor piece_colors[4]);
void drawGrid(vec2 tl, vec2 br);
void drawWalls(void);
vec2 snapToGrid(const vec2 screenPos);

static inline vec2 screen_to_grid(const vec2 s_pos) {
    return vec2_idiv(s_pos, BLOCK_SZ);
}

static inline vec2 grid_to_screen(const vec2 g_pos) {
    return vec2_mul(g_pos, BLOCK_SZ);
}

static inline vec2 local_to_grid(const vec2 l_pos, const vec2 g_origin) {
    return vec2_add(l_pos, g_origin);
}

static const i64 PLAYFIELD_X0 = 5;
static const i64 PLAYFIELD_GXOFFSET = 5;
static const i64 PLAYFIELD_Y0 = 0;
static const i64 SIDE_WALL_WIDTH = 5;
static const i64 PLAYFIELD_WIDTH = 10;
static const i64 PLAYFIELD_HEIGHT = 20;
static const i64 PLAYFIELD_X1 = PLAYFIELD_X0 + PLAYFIELD_WIDTH;
static const i64 PLAYFIELD_Y1 = PLAYFIELD_Y0 + PLAYFIELD_HEIGHT;
static const i64 PLAYFIELD_GYOFFSET = DEF_ROWS - PLAYFIELD_HEIGHT;
