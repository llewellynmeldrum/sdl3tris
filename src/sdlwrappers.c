#include <math.h>

#include "sdlwrappers.h"

const SDL_FColor red = (SDL_FColor){1.0, 0, 0, 1.0};
const SDL_FColor green = (SDL_FColor){0.0, 1.0, 0, 1.0};
const SDL_FColor blue = (SDL_FColor){0.0, 0, 1.0, 1.0};
const SDL_FColor grey = (SDL_FColor){0.3, 0.3, 0.3, 1.0};

SDLContext ctx = {};
SDLContext init_ctx() {
    const size_t RB_CAP = 10000;
    return (SDLContext){
            .window = NULL,
            .renderer = NULL,
            .input = {},
            .perf = {.show = false,
                     .fps_rb = rb_create(sizeof(double), RB_CAP),
                     .ft_rb = rb_create(sizeof(double), RB_CAP)},
            .draw = {.clear_color = makergba(1.0, 1.0, 1.0, 1.0)},
            .clock_freq = NAN,
    };
}
