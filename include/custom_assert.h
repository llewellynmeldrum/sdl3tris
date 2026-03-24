#pragma once
#include "logger.h"
#include "macromagic.h"
#define ASSERT(invariant, fmt, ...)                                                                \
    if (invariant == false) {                                                                      \
        LOGASSERTFAIL(fmt, ##__VA_ARGS__);                                                         \
    }

#define WARNING(this_should_probably_not_be_false, fmt, ...)                                       \
    if (this_should_probably_not_be_false == false) {                                              \
        LOGWARNINGFAIL(fmt, ##__VA_ARGS__);                                                        \
    }
