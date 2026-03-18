#include "primitives.h"
#include "sdlwrappers.h"
#include "sugar.h"

void draw_triangle(SDL_Vertex* vertices, size_t vert_count) {
    SDL_RenderGeometry(ctx.renderer, NULL, vertices, vert_count, NULL, 0);
}

void render_quad(SDL_Vertex* vertices) {
    const int quad_indices[QUAD_IDX_COUNT] = {
        0, 1, 2,  // top left tri
        0, 2, 3   // bottom left tri
    };
    SDL_RenderGeometry(ctx.renderer, NULL, vertices, QUAD_VERT_COUNT, quad_indices,
                       ARRLEN(quad_indices));
}
void make_quad(vec2* points, SDL_FColor color) {
    SDL_Vertex vertices[QUAD_VERT_COUNT] = (SDL_Vertex[]){
        VTX(color, points[0].x, points[0].y),
        VTX(color, points[1].x, points[1].y),
        VTX(color, points[2].x, points[2].y),
        VTX(color, points[3].x, points[3].y),
    };
    render_quad(vertices);
}
