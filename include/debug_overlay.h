#pragma once
#include "types.h"
// src/debugtext.c
void draw_debug_overlay(void);
void overlay_push_str(const char* str);
void overlay_push_fstr(const char* str, ...);
