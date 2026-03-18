#pragma once
#include "sdlwrappers.h"
#include "types.h"

typedef enum {
    // try hovering over (<S>k in neovim) any of the enum values!!!
    PieceType_I_Piece,     // ---> ▌ <---
    PieceType_J_Piece,     // --->▗█<---
    PieceType_L_Piece,     // --->▐▙ <---
    PieceType_O_Piece,     // ---> █ <---
    PieceType_S_Piece,     // --->▗▛ <---
    PieceType_T_Piece,     // ---> ▜▛ <---
    PieceType_Z_Piece,     // --->▝▙ <---
    PieceType_Wall_Piece,  // --->▝▙ <---
} PieceType;

typedef const struct PieceDef {
    const PieceType   type;     // see PieceType
    const SDL_FColor* colors;   // contains increasing in brightness color bands for that piece
    const vec2*       offsets;  // contains grid space offsets of pieces
    const size_t      offsets_len;
    const vec2        l_origin;  // describes the origin of the piece in piece space
} PieceDef;

const PieceDef* get_piece_def(PieceType T);
