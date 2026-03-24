#pragma once
// src/game/internal_draw.c
#include "sugar.h"
#include "types.h"
#include "vec2.h"

void g_drawBlockSpecial(vec2 g_topLeftPos, double extent, const ColorScheme colorscheme,
                        float opacity);

void s_drawBlock(vec2 s_pos, double len, const ColorScheme in_colorscheme, float opacity);
void s_drawPieceSpecial(vec2 s_pos, const vec2* offsets, const ColorScheme colors, float opacity);
void s_drawBlockSpecial(vec2 s_pos, double len, const ColorScheme colorscheme, float opacity);
