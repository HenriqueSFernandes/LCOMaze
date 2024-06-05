#include "../data_structures/stack.h"

void stack_push(struct Stack *stack, struct Cell *cell) {
    struct StackNode *new_node = (struct StackNode *) malloc(sizeof(struct StackNode));
    new_node->cell = cell;
    new_node->next = stack->stack_top;
    stack->stack_top = new_node;
    stack->size++;
}

struct Cell *stack_pop(struct Stack *stack) {
    if (stack->size == 0) {
        return NULL;
    }
    struct StackNode *top_node = stack->stack_top;
    struct Cell *top_cell = top_node->cell;
    stack->stack_top = top_node->next;
    stack->size--;
    return top_cell;
}

struct Cell *stack_top(struct Stack *stack) {
    if (stack->size == 0) {
        return NULL;
    }
    return stack->stack_top->cell;
}
