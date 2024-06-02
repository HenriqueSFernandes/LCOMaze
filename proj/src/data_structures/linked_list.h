/**
 * @file linked_list.h
 * @brief This file contains the declarations of the linked list data structure and its associated functions.
 */

/**
 * \addtogroup data_structures Data Structures
 * @{
 */

/** @defgroup linked_list Linked List
 * @brief Module for the linked list data structure.
 * @{
 */

#ifndef LINKED_LIST_H
#    define LINKED_LIST_H

#    include "../game/maze.h"

/**
 * @struct ListNode
 * @brief A node in a linked list.
 *
 * This struct represents a node in a linked list. It contains a pointer to a `Cell` object and a pointer to the next node in the list.
 */
struct ListNode {
    struct Cell *cell;     ///< Pointer to a `Cell` object.
    struct ListNode *next; ///< Pointer to the next node in the list.
};
/**
 * @struct LinkedList
 * @brief Represents a linked list data structure.
 *
 * The LinkedList structure contains a pointer to the head node, a pointer to the last node,
 * and the size of the linked list.
 */
struct LinkedList {
    struct ListNode *head; ///< Pointer to the head node of the linked list.
    struct ListNode *last; ///< Pointer to the last node of the linked list.
    int size;              ///< The size of the linked list.
};

/**
 * Adds a new cell to the end of the linked list.
 *
 * @param list The linked list to add the cell to.
 * @param cell The cell to be added to the linked list.
 */
void linked_list_push(struct LinkedList *list, struct Cell *cell);

/**
 * Removes and returns the first cell from the linked list.
 *
 * @param list The linked list from which to remove the first cell.
 * @return The first cell that was removed, or NULL if the list is empty.
 */
struct Cell *linked_list_pop_first(struct LinkedList *list);

/**
 * Removes and returns the last cell from the linked list.
 * If the list is empty, returns NULL.
 *
 * @param list The linked list from which to remove the last cell.
 * @return The last cell that was removed, or NULL if the list is empty.
 */
struct Cell *linked_list_pop_last(struct LinkedList *list);

/**
 * Returns the first cell in the linked list.
 *
 * @param list The linked list.
 * @return The first cell in the linked list, or NULL if the list is empty.
 */
struct Cell *linked_list_first(struct LinkedList *list);

/**
 * Returns the last cell of a linked list.
 *
 * @param list The linked list.
 * @return The last cell of the linked list, or NULL if the list is empty.
 */
struct Cell *linked_list_last(struct LinkedList *list);

/**
 * Returns the cell with the minimum distance in the given linked list.
 *
 * @param list The linked list to search for the minimum distance cell.
 * @return The cell with the minimum distance, or NULL if the list is empty.
 */
struct Cell *get_minimum_distance(struct LinkedList *list);

/**
 * Removes a cell from the linked list.
 *
 * @param list The linked list from which to remove the cell.
 * @param cell The cell to be removed from the linked list.
 */
void linked_list_remove(struct LinkedList *list, struct Cell *cell);

/**
 * Checks if a given cell is present in a linked list.
 *
 * @param list The linked list to search in.
 * @param cell The cell to search for.
 * @return true if the cell is found in the linked list, false otherwise.
 */
bool linked_list_contains(struct LinkedList *list, struct Cell *cell);

#endif

/** @} */

/** @} */
