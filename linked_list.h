/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef __LINKED_LIST_H__
#define __LINKED_LIST_H__

typedef struct _linked_list
{
  void * next;
  void * data;
} linked_list;

linked_list * linked_list_append(linked_list * list, void * item);
linked_list * linked_list_delete(linked_list * list, linked_list * p);

linked_list * linked_list_allocate(void * item);
void linked_list_free(linked_list * list);

#endif /* __LINKED_LIST_H__ */
