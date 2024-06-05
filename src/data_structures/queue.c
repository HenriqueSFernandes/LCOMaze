#include "queue.h"
#include <stdlib.h>
#include <stdio.h>

#define BUFFER_SIZE 100

queue_t *new_queue() {
    char *buff = malloc(BUFFER_SIZE * sizeof(char));
    if (buff == NULL) {
        return NULL;
    }
    queue_t *queue = malloc(sizeof(queue_t));
    if (queue == NULL) {
        free(buff);
        return NULL;
    }
    queue->buf = buff;
    queue->front = 0;
    queue->rear = -1;
    queue->size = 0;
    return queue;
}

void delete_queue(queue_t *q) {
    if (q != NULL) {
        free(q->buf);
        free(q);
    }
}

int push(queue_t *q, char c) {
    if (q == NULL || q->size >= BUFFER_SIZE) {
        return 1;
    }
    q->rear = (q->rear + 1) % BUFFER_SIZE;
    q->buf[q->rear] = c;
    q->size++;
    return 0;
}

char top(queue_t *q) {
    if (q == NULL || q->size == 0) {
        return '\0';
    }
    return q->buf[q->front];
}

void pop(queue_t *q) {
    if (q == NULL || q->size == 0) {
        return;
    }
    q->front = (q->front + 1) % BUFFER_SIZE;
    q->size--;
}

int empty(queue_t *q) {
    if (q == NULL) {
        return 1;
    }
    return (q->size == 0);
}
