#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include "../game/maze.h"

struct ListNode {
    struct Cell *cell;
    struct ListNode *next;
};

struct LinkedList {
    struct ListNode *head;
    struct ListNode *last;
    int size;
};

void linked_list_push(struct LinkedList *list, struct Cell *cell);

struct Cell *linked_list_pop_first(struct LinkedList *list);

struct Cell *linked_list_pop_last(struct LinkedList *list);

struct Cell *linked_list_first(struct LinkedList *list);

struct Cell *linked_list_last(struct LinkedList *list);

#endif