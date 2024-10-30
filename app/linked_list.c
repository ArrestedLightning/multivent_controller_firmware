/*
 * linked_list.c
 *
 *  Created on: Oct 27, 2024
 *      Author: jjlange
 *
 *  Basic linked list implementation.
 *  Not particularly thread/interrupt safe.
 */

#include "globals.h"

//#define LL_DBG    1

#ifdef LL_DBG
#define ll_dbg(...)   printf(__VA_ARGS__)
#else
#define ll_dbg(...)
#endif

static ll_node *find_free_ll_node(dll *list) {
    if (list) {
        for (uint32_t i = 0; i < list->max_elements; i += 1) {
            //node is considered free if data pointer is null
            if (list->pool[i].data == NULL) {
                return &list->pool[i];
            }
        }
    }
    return NULL;
}

bool dll_insert_last(dll *list, void *node_data) {
    ll_node *new_node;
    if (list) {
        new_node = find_free_ll_node(list);
        if (new_node) {
            ll_dbg("new: %08x ", new_node);
            ll_dbg("head: %08x, tail: %08x\n", list->head, list->tail);
            new_node->data = node_data;
            if (!list->head) {
                //if there is no existing head element, this is now it
                list->head = new_node;
            }
            if (list->tail) {
                //if there is an existing last element, fix links
                new_node->prev = list->tail;
                list->tail->next = new_node;
            } else {
                new_node->prev = NULL;
            }
            //this is the last element, next is always null
            new_node->next = NULL;
            list->tail = new_node;
            ll_dbg("head2: %08x, tail2: %08x\n", list->head, list->tail);
            return true;
        } else {
            printf("Out of linked list nodes!\n");
        }
    }
    return false;
}

void *dll_get_first(dll *list) {
    if (list && list->head) {
        return list->head->data;
    }
    return NULL;
}

bool dll_remove_start(dll *list) {
    ll_node *head;
    if (list) {
        if (list->head) {
            head = list->head;
            if (list->free_func) {
                list->free_func(head->data);
            }
            list->head = head->next;
            head->next = NULL;
            head->prev = NULL;
            head->data = NULL;//release this node;
            return true;
        }
    }
    return false;
}
