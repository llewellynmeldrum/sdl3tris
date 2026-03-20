#pragma once
// src/game/internal_draw.c
#include "sugar.h"
#include "types.h"
#include "vec2.h"

void g_drawBlock(vec2 gridPos, double len, const ColorScheme piece_colors);
void g_drawBlockSpecial(vec2 g_topLeftPos, double extent, const ColorScheme colorscheme);

void s_drawBlock(vec2 s_pos, double len, const ColorScheme colorscheme);
void s_drawPieceSpecial(vec2 s_pos, const vec2* offsets, const ColorScheme colors);
void s_drawBlockSpecial(vec2 s_pos, double len, const ColorScheme piece_colors);

/*
 *
 *
 *
 *
 *
 *
// later
BlockStyle style); typedef enum { BlockStyle_UARROW, BlockStyle_DARROW, BlockStyle_LARROW,
    BlockStyle_RARROW,
} BlockStyle;

static inline void g_drawBlockX(vec2 gridPos, double len, const SDL_FColor piece_colors[5],
*/
