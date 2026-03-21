#pragma once
#include <SDL3/SDL.h>
#include <stdint.h>

typedef const SDL_FColor* ColorScheme;

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef int64_t i64;
typedef int32_t i32;
typedef int16_t i16;
typedef int8_t  i8;

#define DEF_COLS ((double)20)
#define DEF_ROWS (DEF_HEIGHT / BLOCK_SZ)

#define DEF_WIDTH ((double)640)
#define DEF_HEIGHT ((double)960)
#define BLOCK_SZ (DEF_WIDTH / DEF_COLS)
#define DEF_CELLCOUNT (int)(DEF_COLS * DEF_ROWS)
