/**
 * @file queue.h
 * @brief Defines the queue data structure and its associated functions.
 */

/**
 * \addtogroup data_structures Data Structures
 * @{
 */

/** @defgroup queue Queue
 * @brief Module for the queue data structure.
 * @{
 */

#ifndef __QUEUE_H
#    define __QUEUE_H

/**
 * @brief Structure representing a queue.
 *
 * This structure holds the necessary information to implement a queue data structure.
 */
typedef struct {
    char *buf; ///< Pointer to the array that stores the elements in the queue
    int front; ///< Index of the front element
    int rear;  ///< Index of the last element
    int size;  ///< Number of elements in the queue
} queue_t;

/**
 * @brief Creates a new queue.
 *
 * @return A pointer to the newly created queue.
 */
queue_t *new_queue();

/**
 * @brief Deletes a queue.
 *
 * @param q The queue to be deleted.
 */
void delete_queue(queue_t *q);

/**
 * @brief Pushes an element into the queue.
 *
 * @param q The queue to push the element into.
 * @param c The element to be pushed.
 * @return 0 if successful, -1 if the queue is full.
 */
int push(queue_t *q, char c);

/**
 * @brief Returns the element at the front of the queue without removing it.
 *
 * @param q The queue to get the element from.
 * @return The element at the front of the queue.
 */
char top(queue_t *q);

/**
 * @brief Removes the element at the front of the queue.
 *
 * @param q The queue to remove the element from.
 */
void pop(queue_t *q);

/**
 * @brief Checks if the queue is empty.
 *
 * @param q The queue to check.
 * @return 1 if the queue is empty, 0 otherwise.
 */
int empty(queue_t *q);

#endif
/** @} */

/** @} */
