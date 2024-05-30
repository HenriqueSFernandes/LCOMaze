#include "linked_list.h"

void linked_list_push(struct LinkedList *list, struct Cell *cell) {
    struct ListNode *new_node = (struct ListNode *) malloc(sizeof(struct ListNode));
    new_node->cell = cell;
    new_node->next = NULL;
    if (list->size == 0) {
        list->head = new_node;
        list->last = new_node;
    }
    else {
        list->last->next = new_node;
        list->last = new_node;
    }
    list->size++;
}

struct Cell *linked_list_pop_first(struct LinkedList *list) {
    if (list->size == 0) {
        return NULL;
    }
    struct ListNode *first_node = list->head;
    struct Cell *first_cell = first_node->cell;
    list->head = first_node->next;
    free(first_node);
    list->size--;
    return first_cell;
}

struct Cell *linked_list_pop_last(struct LinkedList *list){
    if (list->size == 0) {
        return NULL;
    }
    struct ListNode *last_node = list->last;
    struct Cell *last_cell = last_node->cell;
    struct ListNode *current_node = list->head;
    while (current_node->next != list->last) {
        current_node = current_node->next;
    }
    current_node->next = NULL;
    list->last = current_node;
    free(last_node);
    list->size--;
    return last_cell;
}

struct Cell *linked_list_first(struct LinkedList *list){
    if (list->size == 0) {
        return NULL;
    }
    return list->head->cell;
}

struct Cell *linked_list_last(struct LinkedList *list){
    if (list->size == 0) {
        return NULL;
    }
    return list->last->cell;
}

struct Cell *get_minimum_distance(struct LinkedList *list){
    if (list->size == 0) {
        return NULL;
    }
    struct ListNode *current_node = list->head;
    struct Cell *min_cell = current_node->cell;
    int min_dist = min_cell->dist;
    while (current_node != NULL) {
        if (current_node->cell->dist < min_dist) {
            min_cell = current_node->cell;
            min_dist = min_cell->dist;
        }
        current_node = current_node->next;
    }
    return min_cell;
}

void linked_list_remove(struct LinkedList *list, struct Cell *cell){
    struct ListNode *current_node = list->head;
    struct ListNode *prev_node = NULL;
    while (current_node != NULL) {
        if (current_node->cell == cell) {
            if (prev_node == NULL) {
                list->head = current_node->next;
            }
            else {
                prev_node->next = current_node->next;
            }
            list->size--;
            return;
        }
        prev_node = current_node;
        current_node = current_node->next;
    }
}

bool linked_list_contains(struct LinkedList *list, struct Cell *cell){
    struct ListNode *current_node = list->head;
    while (current_node != NULL) {
        if (current_node->cell == cell) {
            return true;
        }
        current_node = current_node->next;
    }
    return false;
}
