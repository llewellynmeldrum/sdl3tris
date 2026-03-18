// #include "piecedata.h"
#include "piecedata.h"
#include "sdlwrappers.h"
#include "sugar.h"

// piecedata.c
static const SDL_FColor purple[] = {
    rgb(80, 0, 120), rgb(144, 0, 216), rgb(160, 0, 240), rgb(221, 153, 255), rgb(230, 178, 255),
};
static const SDL_FColor green[] = {
    rgb(0, 120, 0), rgb(0, 216, 0), rgb(0, 240, 0), rgb(153, 255, 153), rgb(178, 255, 178),
};
static const SDL_FColor grey[] = {
    rgb(0, 0, 0), rgb(80, 80, 80), rgb(120, 120, 120), rgb(153, 153, 153), rgb(200, 200, 200),
};
static const SDL_FColor red[] = {
    rgb(120, 0, 0), rgb(216, 0, 0), rgb(240, 0, 0), rgb(255, 153, 153), rgb(255, 178, 178),
};
static const SDL_FColor blue[] = {
    rgb(0, 0, 120), rgb(0, 0, 216), rgb(0, 0, 240), rgb(153, 153, 255), rgb(178, 178, 255),
};
static const SDL_FColor yellow[] = {
    rgb(120, 120, 0), rgb(216, 216, 0), rgb(240, 240, 0), rgb(255, 255, 153), rgb(255, 255, 178),
};
static const SDL_FColor orange[] = {
    rgb(120, 80, 0), rgb(216, 144, 0), rgb(240, 160, 0), rgb(255, 221, 153), rgb(255, 230, 178),
};
static const SDL_FColor cyan[5] = {
    rgb(0, 120, 120), rgb(0, 216, 216), rgb(0, 240, 240), rgb(153, 255, 255), rgb(178, 255, 255),
};

static const vec2 I_Piece_Offsets[4] = {
    { 0, 0 },
    { 0, 1 },
    { 0, 2 },
    { 0, 3 },
};
static const vec2 Wall_Piece_Offsets[1] = {
    { 0, 0 },
};
#define CONCAT(a, b) a##b

#define DECL_PIECE(T, color, ox, oy)                                                               \
    static const PieceDef PieceDef_##T = { .type = PieceType_##T,                                  \
                                           .colors = color,                                        \
                                           .l_origin = { ox, oy },                                 \
                                           .offsets_len = ARRLEN(CONCAT(T, _Offsets)),             \
                                           .offsets = CONCAT(T, _Offsets) }

DECL_PIECE(I_Piece, cyan, 0, 1);
DECL_PIECE(Wall_Piece, grey, 0, 1);

// this is just used to make writing them easier, all are expanded
#define unwrap(T)                                                                                  \
    case PieceType_##T:                                                                            \
        return &(CONCAT(PD_, T));                                                                  \
        break

const PieceDef* get_piece_def(PieceType T) {
    switch (T) {
    case PieceType_I_Piece:
        return &PieceDef_I_Piece;
        break;

    case PieceType_Wall_Piece:
        return &(PieceDef_Wall_Piece);
        break;

    default:
        fprintf(stderr, "Unimplemented, do I_PIECE. Expect a segfault");
        return NULL;
        break;
    }
}
