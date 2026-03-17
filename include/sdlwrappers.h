#pragma once
#include "ringbuffer.h"
#include "types.h"
#include <SDL3/SDL.h>

typedef struct {
    bool m1;
    bool m2;
    vec2 mpos;
} Input;

typedef struct {
    bool        show;
    RingBuffer* fps_rb;
    RingBuffer* ft_rb;
    double      ms_lastframe;
} Perf;

typedef struct {
    SDL_FColor clear_color;
} Draw;
typedef struct {
    SDL_Window*   window;
    SDL_Renderer* renderer;
    Input         input;
    Perf          perf;
    Draw          draw;
    u64           frame_count;
    u64           clock_freq;
} SDLContext;
extern SDLContext ctx;

typedef struct {
    SDL_Vertex* data;
    size_t      size;
} VertexArray;

const extern SDL_FColor red;
const extern SDL_FColor green;
const extern SDL_FColor blue;
const extern SDL_FColor grey;

void       drawVertexArray(VertexArray arr);
SDLContext init_ctx();

#define setrgb(r, g, b) SDL_SetRenderDrawColor(ctx.renderer, r, g, b, 255)
#define setrgba(r, g, b, a) SDL_SetRenderDrawColor(ctx.renderer, r, g, b, a)
#define setcolor(c) SDL_SetRenderDrawColorFloat(ctx.renderer, c.r, c.g, c.b, c.a)
#define getcolor(c) SDL_GetRenderDrawColorFloat(ctx.renderer, &c.r, &c.g, &c.b, &c.a)
#define makergba(r, g, b, a)                                                                       \
    (SDL_FColor) {                                                                                 \
        r, g, b, a                                                                                 \
    }

#define VTX(c, x, y)                                                                               \
    (SDL_Vertex) {                                                                                 \
        .color = c, .position = {x, y}, .tex_coord = {0}                                           \
    }
