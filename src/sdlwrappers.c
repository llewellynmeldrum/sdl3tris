#include <math.h>

#include "sdlwrappers.h"
#include "types.h"

// clang-format off
SDLContext ctx = {};
SDLContext init_ctx() {
    const size_t RB_CAP = 10000;

    return (SDLContext){
        .input = {
            .mgpos = {}
        },

        .perf = {
            .show_perf_in_debug = false,
            .fps_rb = rb_create(sizeof(double), RB_CAP),
            .ft_rb = rb_create(sizeof(double), RB_CAP)
        },

        .draw = {
            .clear_color = rgba(0.0, 0.0, 0.0, 1.0)
        },
        .window = NULL,
        .renderer = NULL,
        .clock_freq = NAN,
        .w = DEF_WIDTH,
        .h = DEF_HEIGHT,
        .cols = DEF_COLS,
        .rows = DEF_ROWS

    };
}
