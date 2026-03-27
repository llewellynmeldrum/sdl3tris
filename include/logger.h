#pragma once

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Piece.h"
#include "piecedata.h"
#include "timing.h"
#include "types.h"
#include "vec2.h"

#define FMT_CLEAR "\e[0m"
#define FMT_BOLD "\e[1m"
#define BOLD_RED "\e[1;31m"
#define BOLD_GREEN "\e[1;32m"
#define BOLD_BLUE "\e[1;33m"
#define BOLD_YELLOW "\e[1;34m"
#define BG_WHITE_FG_BLACK "\e[47;30m"

#define LIGREY "\e[90m"
#define CYAN "\e[34m"
#define GREEN "\e[32m"
#define YELLOW "\e[33m"
#define IYELLOW "\e[7;33m"
#define PINK "\e[35m"
#define LIRED "\e[91m"
#define RED "\e[31m"
#define DWHITE "\e[96m"
#define BOLD_DWHITE "\e[1;96m"
#define UNDERLINE "\e[4m"

#define OSTREAM stdout

static const char* FMT_LOGLEVEL_COLORS[] = {
    GREEN,   // EXIT_SUCCESS
    RED,     // EXIT_FAILURE
    CYAN,    // DEBUG
    LIGREY,  // INFO
    YELLOW,  // NOTICE
    PINK,    // WARN
    LIRED,   // ERROR
    RED,     // FATAL
};
// clang-format off
static const char* loglevel_tostr[] = { 
    "EXIT",
    "EXIT", 
    "DEBUG", 
    "INFO",
    "NOTICE",
    "WARN",
    "ERROR",
    "FATAL"
};

typedef enum LogLevel {
    LogLevel_EXIT_SUCCESS,
    LogLevel_EXIT_FAILURE,
    LogLevel_DEBUG,
    LogLevel_INFO,
    LogLevel_NOTICE,
    LogLevel_WARN,
    LogLevel_ERROR,
    LogLevel_FATAL,
} LogLevel;

static inline void log_trace(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(OSTREAM, fmt, ap);
    va_end(ap);
}

static inline void log_internal(LogLevel level, const char* filename, int line, const char* fmt,
                                ...) {
    double ms = ms_since_start();
    log_trace(FMT_CLEAR);
    log_trace("%06.3lfs ", ms / 1000.0);
    log_trace(FMT_LOGLEVEL_COLORS[level]);
    log_trace("%-8s", loglevel_tostr[level]);
    log_trace(FMT_CLEAR);

    log_trace(BOLD_DWHITE);
    log_trace("%s:%d: ", filename, line);
    log_trace(FMT_CLEAR);
    va_list ap;
    va_start(ap, fmt);
    vfprintf(OSTREAM, fmt, ap);
    va_end(ap);
    fprintf(OSTREAM, "\n");
    if (level==LogLevel_EXIT_FAILURE || level==LogLevel_EXIT_SUCCESS){
        exit(level);
    }
}

typedef struct LogSettings {
    bool showFunctions;
} LogSettings;
static LogSettings log_settings;
#define SETLOG_SHOWFUNCTIONS(val) log_settings.showFunctions = val;
// clang-format off
#define LOG_DEBUG(fmt, ...)         log_internal(LogLevel_DEBUG, __FILE_NAME__, __LINE__, fmt,  ##__VA_ARGS__)
#define LOG_INFO(fmt, ...)          log_internal(LogLevel_INFO, __FILE_NAME__, __LINE__, fmt,   ##__VA_ARGS__)
#define LOG_NOTICE(fmt, ...)        log_internal(LogLevel_NOTICE, __FILE_NAME__, __LINE__, fmt, ##__VA_ARGS__)
#define LOG_WARN(fmt, ...)          log_internal(LogLevel_WARN, __FILE_NAME__, __LINE__, fmt,   ##__VA_ARGS__)
#define LOG_ERROR(fmt, ...)         log_internal(LogLevel_ERROR, __FILE_NAME__, __LINE__, fmt,  ##__VA_ARGS__)
#define LOG_FATAL(fmt, ...)         log_internal(LogLevel_FATAL, __FILE_NAME__, __LINE__, fmt,  ##__VA_ARGS__)
#define LOG_EXIT(code) log_internal(code,__FILE_NAME__, __LINE__, "Exiting. (Code:%d)",code)
// clang-format on

#define X_LIST_TYPENAMES                                                                           \
    X(int, "%d", val)                                                                              \
    X(uint64_t, "%llu", val)                                                                       \
    X(double, "%lf", val)                                                                          \
    X(vec2, "%.2f,%.2f", val.x, val.y)                                                             \
    X(bool, "%s", (val ? "true" : "false"))                                                        \
    X(SDL_FColor, "rgba(%f,%f,%f,%f)", val.r, val.g, val.b, val.a)                                 \
    X(Piece, "{\n\t.T=%s, \n\t.rotation=%s, \n\t.g_pos={%s}\n}", pt_toStr(val.T),                  \
      Direction_tostr(val.rotation), vec2_toStr(val.g_pos))                                        \
    X(ColorScheme, "{\n\t.rgb[0]=%s\n}", SDL_FColor_toStr(val[0]))

#define X(T, fmt, ...)                                                                             \
    static inline const char* T##_toStr(T val) {                                                   \
        char* buf = calloc(128, sizeof(char));                                                     \
        fprintf(stderr, "%s ", #T);                                                                \
        snprintf(buf, 128, fmt, ##__VA_ARGS__);                                                    \
        return buf;                                                                                \
    }

X_LIST_TYPENAMES
#undef X

// clang-format off
#define TOSTR(x) \
    _Generic(x, \
        int: int_toStr, \
        uint64_t: uint64_t_toStr, \
        double: double_toStr, \
        bool: bool_toStr, \
        vec2: vec2_toStr, \
        Piece: Piece_toStr, \
        SDL_FColor: SDL_FColor_toStr, \
        ColorScheme: ColorScheme_toStr, \
        default: int_toStr)(x)
// clang-format on

#define LOG_EXPR(x)                                                                                \
    do {                                                                                           \
        const char* str = TOSTR(x);                                                                \
        log_internal(LogLevel_INFO, __FILE_NAME__, __LINE__, "%s = %s", #x, str);                  \
        free((void*)str);                                                                          \
    } while (0)
