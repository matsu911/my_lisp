/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

#include "type.h"

typedef struct _linked_list
{
  void * next;
  void * data;
} linked_list;

linked_list * linked_list_append(linked_list * list, void * item);
linked_list * linked_list_delete(linked_list * list, linked_list * p);

linked_list * linked_list_allocate(void * item);
void linked_list_free(linked_list * list);

int linked_list_size(linked_list * list);
void linked_list_traverse(linked_list * list, void (*func)(linked_list *));
void linked_list_traverse_free(linked_list * list);
linked_list * linked_list_find(linked_list * list, void * val, boolean (*matcher)(void*, void*));

#endif /* __LINKED_LIST_H__ */
