#include "game/internal_draw.h"
#include "primitives.h"

void g_drawBlock(vec2 g_pos, double extent, const ColorScheme colorscheme) {
    vec2 s_topLeftPos = grid_to_screen(g_pos);
    s_drawBlock(s_topLeftPos, extent, colorscheme);
}

void g_drawBlockSpecial(vec2 g_topLeftPos, double extent, const ColorScheme colorscheme) {
    // given a grid position, draw a block with an X over it from the origin
    vec2 s_topLeftPos = grid_to_screen(g_topLeftPos);
    s_drawBlockSpecial(s_topLeftPos, extent, colorscheme);
}

void s_drawBlockSpecial(vec2 s_pos, double len, const ColorScheme colorscheme) {
    SDL_FColor translucent_colors[5] = {};
    for (int i = 0; i < 5; i++) {
        translucent_colors[i] = colorscheme[i];
        translucent_colors[i].a = 0.5;
    }
    s_drawBlock(s_pos, len, translucent_colors);
    float      thick = len * 0.14;
    SDL_FColor black = (SDL_FColor){ 0.1, 0.1, 0.1, 0.5 };

    vec2 top_left = { s_pos.x, s_pos.y };
    vec2 bot_right = { s_pos.x + len, s_pos.y + len };
    vec2 top_right = { s_pos.x + len, s_pos.y };
    vec2 bot_left = { s_pos.x, s_pos.y + len };
    {
        // TL->BR LINE
        // clang-format off
        vec2 quad_verts[] = {
            { top_left.x,           top_left.y + thick  },
            { top_left.x + thick,   top_left.y          },
            { bot_right.x,          bot_right.y - thick },
            { bot_right.x - thick,  bot_right.y         },
        };
        s_drawQuad(quad_verts, black);
    }
    {
        // TR->BL LINE
        vec2 quad_verts[] = {
            { top_right.x - thick, top_right.y },
            { top_right.x, top_right.y + thick },
            { bot_left.x + thick, bot_left.y },
            { bot_left.x, bot_left.y - thick },
        };
        s_drawQuad(quad_verts, black);
    }
}
void s_drawBlock(vec2 s_pos, double len, const ColorScheme colorscheme) {
    double bord = len * 0.14;  // border width
    {
        // MAIN QUAD
        vec2 quad_verts[] = {
            s_pos,
            { s_pos.x + len, s_pos.y },
            { s_pos.x + len, s_pos.y + len },
            { s_pos.x, s_pos.y + len },
        };
        s_drawQuad(quad_verts, colorscheme[2]);
    }
    {
        // LEFT EDGE
        vec2 o = { s_pos.x, s_pos.y };
        vec2 quad_verts[] = {
            o,
            { o.x + bord, o.y + bord },
            { o.x + bord, o.y + len - bord },
            { o.x, o.y + len },
        };
        s_drawQuad(quad_verts, colorscheme[1]);
    }
    {
        // RIGHT EDGE
        vec2 o = { s_pos.x + len - bord, s_pos.y };
        vec2 quad_verts[] = {
            { o.x, o.y + bord },
            { o.x + bord, o.y },
            { o.x + bord, o.y + len },
            { o.x, o.y + len - bord },
        };
        s_drawQuad(quad_verts, colorscheme[1]);
    }
    {
        // TOP EDGE
        vec2 o = { s_pos.x, s_pos.y };
        vec2 quad_verts[] = {
            { o.x, o.y },
            { o.x + len, o.y },
            { o.x + len - bord, o.y + bord },
            { o.x + bord, o.y + bord },
        };
        s_drawQuad(quad_verts, colorscheme[3]);
    }
    {
        // BOT EDGE
        vec2 o = { s_pos.x, s_pos.y + len };
        vec2 quad_verts[] = {
            { o.x, o.y },
            { o.x + bord, o.y - bord },
            { o.x + len - bord, o.y - bord },
            { o.x + len, o.y },
        };
        s_drawQuad(quad_verts, colorscheme[0]);
    }
}
