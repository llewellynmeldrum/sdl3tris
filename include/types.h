#pragma once
#include <stdint.h>

#include "sugar.h"

#pragma clang diagnostic ignored "-Wmissing-braces"
typedef struct {
    union {
        struct {
            float x;
            float y;
        };
        struct {
            float width;
            float height;
        };
    };
} vec2;
