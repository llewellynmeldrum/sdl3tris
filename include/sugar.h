#pragma once
#include "types.h"
#include <assert.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>

#define vec2_unpack(p) p.x, p.y

static inline vec2 rotate_vec2(vec2 a, double theta) {
    return (vec2){ .x = a.x * cos(theta) - a.y * sin(theta),
                   .y = a.x * sin(theta) + a.y * cos(theta) };
}
// piecewise min on both components
static inline vec2 vec2_min(vec2 a, vec2 b) {
    return (vec2){
        .x = fmin(a.x, b.x),
        .y = fmin(a.y, b.y),
    };
}

// piecewise max on both components
static inline vec2 vec2_max(vec2 a, vec2 b) {
    return (vec2){
        .x = fmax(a.x, b.x),
        .y = fmax(a.y, b.y),
    };
}
// piecewise subtraction between two vectors
#define vec2_sub(a, b)                                                                             \
    (vec2) {                                                                                       \
        a.x - b.x, a.y - b.y                                                                       \
    }
// piecewise addition between two vectors
#define vec2_add(a, b)                                                                             \
    (vec2) {                                                                                       \
        a.x + b.x, a.y + b.y                                                                       \
    }
// piecewise mult between a vector and a scalar
#define vec2_mul(v, s)                                                                             \
    (vec2) {                                                                                       \
        v.x *s, v.y *s                                                                             \
    }

// regular real division
#define vec2_div(v, s)                                                                             \
    (vec2) {                                                                                       \
        v.x / s, v.y / s                                                                           \
    }

// truncating integer division
#define vec2_idiv(v, s)                                                                            \
    (vec2) {                                                                                       \
        (i64) v.x / (i64)s, (i64)v.y / (i64)s                                                      \
    }
#define vec2_print(v) printf("%s:{%f,%f}\n", #v, v.x, v.y)

#define ARRLEN(arr) (sizeof(arr) / sizeof(arr[0]))

#define LEN(expr) sizeof(expr)  // been using too much python

#define bound(val, lo, hi) (fmin(hi, (fmax(lo, val))))

static inline u64 urand(u64 min, u64 max) {
    u64 range = max - min;
    assert(range >= 0 || "Bro");
    return (min + (rand() % range));
}
