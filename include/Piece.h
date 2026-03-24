#pragma once
#include "PieceType.h"
#include "types.h"
#include "vec2.h"

typedef struct Piece {
    PieceType T;
    Direction rotation;
    vec2      g_pos;
} Piece;
