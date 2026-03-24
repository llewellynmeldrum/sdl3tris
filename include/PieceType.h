#pragma once
#define PieceType_START PieceType_I_Piece
typedef enum {
    // try hovering over (<S>k in neovim) any of the enum values!!!
    PieceType_I_Piece,     // ---> ▌ <---
    PieceType_J_Piece,     // --->▗█<---
    PieceType_L_Piece,     // --->▐▙ <---
    PieceType_O_Piece,     // ---> █ <---
    PieceType_S_Piece,     // --->▗▛ <---
    PieceType_T_Piece,     // ---> ▜▛ <---
    PieceType_Z_Piece,     // --->▝▙ <---
    PieceType_Wall_Piece,  // ---> WALL <---
} PieceType;
#define PieceType_END PieceType_Z_Piece
#define PieceType_COUNT (PieceType_Z_Piece + 1)
