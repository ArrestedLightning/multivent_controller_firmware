/*
 * linked_list.h
 *
 *  Created on: Oct 27, 2024
 *      Author: jjlange
 */

#ifndef APP_LINKED_LIST_H_
#define APP_LINKED_LIST_H_

typedef void (*_free_func)(void*);

typedef struct Node ll_node;

/* Doubly- (or singly-) linked list node structure */
typedef struct Node {
    void* data;
    ll_node* next;
    ll_node* prev;
} ll_node;

/* Doubly-linked list structure */
typedef struct {
    ll_node *head;
    ll_node *tail;
    uint32_t max_elements;
    _free_func free_func;
    ll_node *pool;
} dll;

#define DLL_ALLOC(name, num, free_fn) ll_node name##_##backing[num]; dll name = \
    {.head = NULL,\
     .tail = NULL,\
     .max_elements = num,\
     .pool = name##_##backing,\
     .free_func = free_fn};

bool dll_insert_last(dll *list, void *node_data);
void *dll_get_first(dll *list);
bool dll_remove_start(dll *list);

#endif /* APP_LINKED_LIST_H_ */
