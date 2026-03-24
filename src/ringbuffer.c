#include "ringbuffer.h"
#include "logger.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>

RingBuffer* rb_create(i64 element_size, i64 capacity) {
    RingBuffer* ptr = calloc(1, sizeof(*ptr));
    ptr->element_size = element_size;
    ptr->capacity = capacity;
    ptr->data = calloc(capacity, element_size);
    ptr->count = 0;
    return ptr;
}

void rb_insert(RingBuffer* rb, void* element, i64 pos) {
    void* src = element;
    void* dst = (char*)rb->data + (pos * rb->element_size);
    i64   size = rb->element_size;
    memmove(dst, src, size);
}

void* rb_get(RingBuffer* rb, i64 pos) {
    return (char*)rb->data + (pos * rb->element_size);
}

void* rb_peek(RingBuffer* rb) {
    return (char*)rb->data + ((rb->count - 1) * rb->element_size);
}

void* rb_pop(RingBuffer* rb) {
    void* res = rb_get(rb, rb->count - 1);
    rb->count--;
    return res;
}

void rb_push(RingBuffer* rb, void* element) {
    rb_insert(rb, element, rb->count);
    rb->count = (rb->count + 1) % rb->capacity;
}

void rb_destory(RingBuffer* rb) {
    free(rb->data);
}

double dbl_rb_avg(RingBuffer* rb) {
    // assumes rb is double
    double sum = 0;
    for (i64 i = 0; i < rb->count; i++) {
        sum += *(double*)rb_get(rb, i);
    }
    double avg = sum / rb->count;
    return avg;
}
