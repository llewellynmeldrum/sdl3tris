#pragma once
#include <sys/time.h>

double get_current_ms();
/*
typedef struct timeval Timeval;

inline void RecordTime(Timeval t) {
    gettimeofday(&t, NULL);
}

inline Timeval GetTime() {
    Timeval t;
    gettimeofday(&t, NULL);
    return t;
}

#define BenchmarkFunction(span, callable, ...)                                                     \
    do {                                                                                           \
        Timeval before, after;                                                                     \
        RecordTime(before);                                                                        \
        callable(__VA_ARGS__);                                                                     \
        RecordTime(after);                                                                         \
        span = GetTimespan(before, after);                                                         \
    } while (0)

typedef struct timespan {
    time_t m;
    time_t s;
    time_t ms;
    time_t us;
} Timespan;

Timespan getTimespan(Timeval before, Timeval after);
void     printTimespan(Timespan t);
*/
