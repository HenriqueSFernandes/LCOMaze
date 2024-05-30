#include "stack.h"

void push(struct Stack *stack, struct Cell *cell) {
    struct StackNode *new_node = (struct StackNode *) malloc(sizeof(struct StackNode));
    new_node->cell = cell;
    new_node->next = stack->top;
    stack->top = new_node;
    stack->size++;
}

struct Cell *pop(struct Stack *stack) {
    if (stack->size == 0) {
        return NULL;
    }
    struct StackNode *top_node = stack->top;
    struct Cell *top_cell = top_node->cell;
    stack->top = top_node->next;
    stack->size--;
    return top_cell;
}

struct Cell *top(struct Stack *stack) {
    if (stack->size == 0) {
        return NULL;
    }
    return stack->top->cell;
}
