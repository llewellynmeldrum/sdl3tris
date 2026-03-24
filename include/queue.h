#pragma once
#include "types.h"
#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
typedef struct QueueNode {
    struct QueueNode* next;
    struct QueueNode* prev;
    void*             data;
} QueueNode;

typedef struct Queue {
    QueueNode* head;
    QueueNode* tail;
    i64        size;
    const i64  element_size;
} Queue;

static inline void debug_print_q(Queue* q) {
    QueueNode* cur = q->tail;
    printf("[");
    while (cur) {
        printf("%p->", cur->data);
        cur = cur->next;
        printf(", ");
    }
    printf("]\n");
    printf("Queue located @%p:\n", q);
    printf("Queue{\n"
           "    QueueNode*    front = %p;\n"
           "    QueueNode*     back = %p;\n"
           "    i64         size = %lld;\n"
           "    i64 element_size = %lld;\n"
           "}\n",
           q->head, q->tail, q->size, q->element_size);
}
Queue pq_create(const i64 element_size);
void  pq_destroy(Queue* q);

// peeks from front of queue (FIFO)
void* pq_peek(Queue* q);
// pops from front of queue (FIFO)
void* pq_pop(Queue* q);
// pushes to back of queue
void pq_push(Queue* q, void* element);
void q_print(Queue* q, void (*print_function)(void*));
