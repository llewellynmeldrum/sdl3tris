#include "sugar.h"
#include <math.h>
#include <stdlib.h>
#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include "debugtext.h"
#include "ringbuffer.h"
#include "sdlwrappers.h"
#include "sugar.h"
#include "timing.h"
#include "types.h"
void draw_triangle(SDL_Vertex* tri_verts, size_t vert_count);

#define DrawTriangle(vertices) draw_triangle(vertices, arrlen(vertices))

void draw_triangle(SDL_Vertex* vertices, size_t vert_count) {
    SDL_RenderGeometry(ctx.renderer, NULL, vertices, vert_count, NULL, 0);
}

#define DrawQuad(vertices) draw_quad(vertices, arrlen(vertices))
void draw_quad(SDL_Vertex* vertices, size_t vert_count) {
    const int quad_indices[] = {
            0, 1, 2,  // top left tri
            0, 2, 3   // bottom left tri
    };
    SDL_RenderGeometry(ctx.renderer, NULL, vertices, vert_count, quad_indices,
                       arrlen(quad_indices));
}

SDL_AppResult SDL_AppIterate(void* _) {
    ctx.frame_count++;
    double ms_thisframe = get_current_ms();
    double ft_ms = ms_thisframe - ctx.perf.ms_lastframe;
    double fps = 1000.0 / (ft_ms);
    rb_push(ctx.perf.ft_rb, &ft_ms);
    rb_push(ctx.perf.fps_rb, &fps);

    setcolor(ctx.draw.clear_color);
    SDL_RenderClear(ctx.renderer);

    // frame starts here

    SDL_Vertex tri_verts[] = {
            VTX(red, 200, 75),
            VTX(green, 100, 225),
            VTX(blue, 300, 225),
    };
    DrawTriangle(tri_verts);

    SDL_Vertex quad_verts[] = {
            VTX(red, 300, 100),
            VTX(green, 400, 130),
            VTX(blue, 400, 300),
            VTX(grey, 300, 330),
    };
    if (ctx.input.m1) {
        quad_verts[0] = VTX(red, ctx.input.mpos.x, ctx.input.mpos.y);
    }
    DrawQuad(quad_verts);

    drawDebugWindow();
    SDL_RenderPresent(ctx.renderer);
    ctx.perf.ms_lastframe = ms_thisframe;
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppInit(void** _, int argc, char* argv[]) {
    ctx = init_ctx();
    SDL_SetAppMetadata("title", "1.0", "com.example.hi");

    if (!SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS | SDL_INIT_AUDIO)) {
        SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    if (!SDL_CreateWindowAndRenderer("examples/renderer/clear", 640, 480, SDL_WINDOW_RESIZABLE,
                                     &ctx.window, &ctx.renderer)) {
        SDL_Log("Couldn't create window/renderer: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }
    SDL_SetRenderLogicalPresentation(ctx.renderer, 640, 480, SDL_LOGICAL_PRESENTATION_LETTERBOX);

    ctx.clock_freq = SDL_GetPerformanceFrequency();
    return SDL_APP_CONTINUE; /* carry on with the program! */
}

void printmpos() {
    double x = ctx.input.mpos.x;
    double y = ctx.input.mpos.y;
    printf("CLICK @: %.2f, %.2f\n", x, y);
}
SDL_AppResult SDL_AppEvent(void* appstate, SDL_Event* event) {
    (void)appstate;
    switch (event->type) {
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        ctx.input.m1 = (event->button.button == 1) ? true : ctx.input.m1, printmpos();
        ctx.input.m2 = (event->button.button == 2) ? true : ctx.input.m2;
        break;
    case SDL_EVENT_MOUSE_BUTTON_UP:
        ctx.input.m1 = (event->button.button == 1) ? false : ctx.input.m1;
        ctx.input.m2 = (event->button.button == 2) ? false : ctx.input.m2;
        break;

    case SDL_EVENT_MOUSE_MOTION:
        ctx.input.mpos = (vec2){event->motion.x, event->motion.y};
        break;

    case SDL_EVENT_QUIT:  //
        return SDL_APP_SUCCESS;
        break;

    default:  //
        return SDL_APP_CONTINUE;
        break;
    }
    return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void* appstate, SDL_AppResult result) {
    (void)appstate;
    (void)result;
    /* SDL will clean up the window/renderer for us. */
}
