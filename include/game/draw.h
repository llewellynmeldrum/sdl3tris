#pragma once
#include "piecedata.h"
#include "sdlwrappers.h"

void drawPiece(vec2 g_originPos, PieceType T);

void drawBlockV(vec2 screenPos, double len, const SDL_FColor piece_colors[4]);
void drawBlockOutlineV(vec2 screenPos, double len, const SDL_FColor piece_colors[4]);
void draw_block(double tlx, double tly, double len, const SDL_FColor piece_colors[4]);
void drawGrid();
void drawBoundaries();
void drawTetro(vec2 s_pos, const vec2* offsets, const SDL_FColor* colors);
vec2 snapToGrid(const vec2 screenPos);
