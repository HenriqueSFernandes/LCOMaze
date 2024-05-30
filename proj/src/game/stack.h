#ifndef STACK_H
#define STACK_H

#include "maze.h"

struct StackNode {
  struct Cell* cell;
  struct StackNode* next;
};

struct Stack {
  struct StackNode* top;
  int size;
};

void push(struct Stack* stack, struct Cell* cell);

struct Cell* pop(struct Stack* stack);

struct Cell* top(struct Stack* stack);

#endif
