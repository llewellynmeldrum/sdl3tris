#pragma once
#include "logger.h"
#include "macromagic.h"
#define ASSERT(invariant, fmt, ...)                                                                \
    if (invariant == false) {                                                                      \
        LOG_FATAL("Assertion Failed:", fmt, ##__VA_ARGS__);                                        \
    }

#define ASSERT_WARNING(this_should_probably_not_be_false, fmt, ...)                                \
    if (this_should_probably_not_be_false == false) {                                              \
        LOG_WARN(fmt, ##__VA_ARGS__);                                                              \
    }
