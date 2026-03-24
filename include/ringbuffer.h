#pragma once
#include "types.h"
#include <stddef.h>
typedef struct RingBuffer {
    void* data;
    i64   element_size;
    i64   capacity;

    i64 count;
} RingBuffer;

RingBuffer* rb_create(i64 element_size, i64 capacity);
void        rb_destory(RingBuffer* rb);

void   rb_insert(RingBuffer* rb, void* element, i64 pos);
void*  rb_get(RingBuffer* rb, i64 pos);
void*  rb_peek(RingBuffer* rb);
void*  rb_pop(RingBuffer* rb);
void   rb_push(RingBuffer* rb, void* element);
double dbl_rb_avg(RingBuffer* rb);
