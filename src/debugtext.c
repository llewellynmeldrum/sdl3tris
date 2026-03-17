#include <SDL3/SDL.h>
#include <math.h>
#include <stdlib.h>

#include "ringbuffer.h"
#include "sdlwrappers.h"
#include "types.h"

void drawDebugText(vec2* pos, float txt_scale, const char* fmt, ...) {
    const float BASE_PX = 8.0;
    const float GAP_PX = 2.0;

    SDL_FColor previous_color;
    getcolor(previous_color);
    setrgb(255, 0, 255);
    txt_scale /= BASE_PX;

    char    buf[1000];
    va_list args;
    va_start(args, fmt);
    vsnprintf(buf, len(buf), fmt, args);
    va_end(args);

    SDL_SetRenderScale(ctx.renderer, txt_scale, txt_scale);
    SDL_RenderDebugText(ctx.renderer, pos->x / txt_scale, pos->y / txt_scale, buf);
    //    pos->x += txt_scale * strlen(buf) * BASE_PX;
    pos->y += GAP_PX + txt_scale * BASE_PX;
    SDL_SetRenderScale(ctx.renderer, 1.0, 1.0);
    setcolor(previous_color);
}

void drawDebugWindow() {
    vec2 tpos = {4, 4};
    vec2 mpos = {ctx.input.mpos.x, ctx.input.mpos.y};
    drawDebugText(&tpos, 14.0, "mpos: %.4f, %.4f", mpos.x, mpos.y);
    drawDebugText(&tpos, 14.0, "m1: %s", ctx.input.m1 ? "DOWN" : "UP");

    if (ctx.perf.show) {
        drawDebugText(&tpos, 14.0, "frametime: %.4lf", dbl_rb_avg(ctx.perf.ft_rb));
        drawDebugText(&tpos, 14.0, "framecount: %ld", ctx.frame_count);
        drawDebugText(&tpos, 14.0, "fps: %.1lf", dbl_rb_avg(ctx.perf.fps_rb));
    }
}
