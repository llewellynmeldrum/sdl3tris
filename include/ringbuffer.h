#pragma once
#include <stddef.h>
typedef struct RingBuffer {
    void*  data;
    size_t element_size;
    size_t capacity;

    size_t count;
} RingBuffer;

RingBuffer* rb_create(size_t element_size, size_t capacity);
void        rb_destory(RingBuffer* rb);

void   rb_insert(RingBuffer* rb, void* element, size_t pos);
void*  rb_get(RingBuffer* rb, size_t pos);
void*  rb_peek(RingBuffer* rb);
void*  rb_pop(RingBuffer* rb);
void   rb_push(RingBuffer* rb, void* element);
double dbl_rb_avg(RingBuffer* rb);
