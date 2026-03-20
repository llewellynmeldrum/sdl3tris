#pragma once  // src/primitives.c
#include "sdlwrappers.h"
#define QUAD_VERT_COUNT 4
#define QUAD_IDX_COUNT 6
#define DrawTriangle(vertices) draw_triangle(vertices, ARRLEN(vertices))
#define DrawLine(p1, p2) SDL_RenderLine(ctx.renderer, p1.x, p1.y, p2.x, p2.y)
#define DrawQuad(vertices, color) make_quad(vertices, color)

void draw_triangle(SDL_Vertex* vertices, size_t vert_count);
void s_drawQuad(vec2* points, SDL_FColor color);
