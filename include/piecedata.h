#pragma once
#include "PieceType.h"
#include "sdlwrappers.h"
#include "types.h"

// NOTE: implementation in:
// "src/piecedata.c"

#define ROTATION_COUNT 4
#define BLOCKS_PER_PIECE 4
#define COLORBAND_COUNT 5

static inline i64 getBlockOffset(i64 rotation_idx, i64 block_idx) {
    return rotation_idx * ROTATION_COUNT + block_idx;
}
typedef const struct PieceData {
    const PieceType   type;         // see PieceType
    const SDL_FColor* colorscheme;  // contains increasing in brightness color bands for that piece
    const vec2*       l_blockOffsets;  // **!USE GET_BLOCK_IDX(rotation_idx, block_idx) TO INDEX!**
    const vec2        l_rot_origin;    // describes the origin of the piece in piece space
    const i64         rotation_count;
    const vec2        l_boundingBox;
} PieceData;

const PieceData*        getPieceData(PieceType T);
const char*             pt_toStr(PieceType T);
static const SDL_FColor WHITE[] = {
    rgb(200, 200, 200), rgb(220, 220, 220), rgb(230, 230, 230),
    rgb(240, 240, 240), rgb(250, 255, 255),
};
// piecedata.c
static const SDL_FColor purple_bands[] = {
    rgb(80, 0, 120), rgb(144, 0, 216), rgb(160, 0, 240), rgb(221, 153, 255), rgb(230, 178, 255),
};
static const SDL_FColor green_bands[] = {
    rgb(0, 120, 0), rgb(0, 216, 0), rgb(0, 240, 0), rgb(153, 255, 153), rgb(178, 255, 178),
};
static const SDL_FColor grey_bands[] = {
    rgb(0, 0, 0), rgb(80, 80, 80), rgb(120, 120, 120), rgb(153, 153, 153), rgb(200, 200, 200),
};
#define DIMNESS 24
static const SDL_FColor grey_dim_bands[] = {
    rgba(0, 0, 0, DIMNESS),       rgba(80, 80, 80, DIMNESS),    rgba(120, 120, 120, DIMNESS),
    rgba(153, 153, 153, DIMNESS), rgba(200, 200, 200, DIMNESS),
};
static const SDL_FColor red_bands[] = {
    rgb(120, 0, 0), rgb(216, 0, 0), rgb(240, 0, 0), rgb(255, 153, 153), rgb(255, 178, 178),
};
static const SDL_FColor blue_bands[] = {
    rgb(0, 0, 120), rgb(0, 0, 216), rgb(0, 0, 240), rgb(153, 153, 255), rgb(178, 178, 255),
};
static const SDL_FColor yellow_bands[] = {
    rgb(120, 120, 0), rgb(216, 216, 0), rgb(240, 240, 0), rgb(255, 255, 153), rgb(255, 255, 178),
};
static const SDL_FColor orange_bands[] = {
    rgb(120, 80, 0), rgb(216, 144, 0), rgb(240, 160, 0), rgb(255, 221, 153), rgb(255, 230, 178),
};
static const SDL_FColor cyan_bands[] = {
    rgb(0, 120, 120), rgb(0, 216, 216), rgb(0, 240, 240), rgb(153, 255, 255), rgb(178, 255, 255),
};
