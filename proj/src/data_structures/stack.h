/**
 * @file stack.h
 * @brief Defines the stack data structure and its associated functions.
 */

/**
 * \addtogroup data_structures Data Structures
 * @{
 */

/** @defgroup stack Stack
 * @brief Module for the stack data structure.
 * @{
 */

#ifndef STACK_H
#    define STACK_H

#    include "../game/maze.h"

/**
 * @struct StackNode
 * @brief Represents a node in a stack data structure.
 *
 * Each node contains a pointer to a Cell structure and a pointer to the next node in the stack.
 */
struct StackNode {
    struct Cell *cell;      ///< Pointer to the Cell structure stored in this node.
    struct StackNode *next; ///< Pointer to the next node in the stack.
};

/**
 * @struct Stack
 * @brief Represents a stack data structure.
 *
 * The Stack struct contains a pointer to the top node of the stack and the size of the stack.
 */
struct Stack {
    struct StackNode *stack_top; ///< Pointer to the top node of the stack.
    int size;                    ///< Size of the stack.
};

/**
 * @brief Pushes a new cell onto the stack.
 *
 * This function creates a new StackNode, sets its cell field to the provided cell pointer,
 * and its next field to the current top of the stack. It then updates the top of the stack
 * to be the new node.
 *
 * @param stack A pointer to the stack where the new element should be added.
 * @param cell A pointer to the element that should be added to the stack.
 */
void stack_push(struct Stack *stack, struct Cell *cell);

/**
 * Removes and returns the top cell from the stack.
 * If the stack is empty, returns NULL.
 *
 * @param stack The stack from which to pop the top cell.
 * @return The top cell that was removed from the stack, or NULL if the stack is empty.
 */
struct Cell *stack_pop(struct Stack *stack);

/**
 * Returns the top element of the stack.
 *
 * @param stack The stack to retrieve the top element from.
 * @return The top element of the stack, or NULL if the stack is empty.
 */
struct Cell *stack_top(struct Stack *stack);

#endif

/** @} */

/** @} */
