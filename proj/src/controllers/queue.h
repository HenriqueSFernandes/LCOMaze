#ifndef __QUEUE_H
#define __QUEUE_H

typedef struct {
    char *buf;   // Pointer to the array that stores the elements in the queue
    int front;   // Index of the front element
    int rear;    // Index of the last element
    int size;    // Number of elements in the queue
} queue_t;

queue_t* new_queue();
void delete_queue(queue_t* q);
int push(queue_t* q, char c);
char top(queue_t* q);
void pop(queue_t* q);
int empty(queue_t* q);

#endif
