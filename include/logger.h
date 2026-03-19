#pragma once

// clang-format off
#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "types.h"
// Given two heap allocated strings, insert `src` starting at `dst[pos]`.
// dst is reallocated to be the size of the two strings combined, i.e strlen(src)+strlen(dst)+1.
// (The null pointer of src is not copied.
#define LOGLN(fmt, ...)                                                                              \
    fprintf(stderr, "\e[1m[%s:%d]:\e[0m" fmt "\n", __FILE__, __LINE__ __VA_OPT__(,) __VA_ARGS__)

#define LOG(fmt, ...)                                                                              \
    fprintf(stderr, "\e[1m[%s:%d]:\e[0m" fmt, __FILE__, __LINE__ __VA_OPT__(,) __VA_ARGS__)
#define LOGERR(fmt, ...)                                                                           \
    fprintf(stderr, "[ERROR] in %s:%d->" fmt, __FILE__, __LINE__, ##__VA_ARGS__)
