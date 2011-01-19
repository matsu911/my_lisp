/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include <stdlib.h>
#include "linked_list.h"
#include "unittest.h"

linked_list * linked_list_append(linked_list * list, void * item)
{
  linked_list * p = linked_list_allocate(item);
  if(list->next)
    p->next = list->next;
  p->data = item;
  list->next = p;

  return p;
}

linked_list * linked_list_delete(linked_list * list, linked_list * p)
{
  /* TODO: implement */
  return NULL;
}

linked_list * linked_list_allocate(void * item)
{
  linked_list * p = (linked_list*)malloc(sizeof(linked_list));

  p->next = NULL;
  p->data = item;

  return p;
}

void linked_list_free(linked_list * list)
{
  if(list)
    free(list);
}

linked_list * linked_list_nth(linked_list * list, const int n)
{
  linked_list * p = list;

  for(int i = 0; i < n; ++i)
  {
    if(p->next == NULL)
      return NULL;
    p = p->next;
  }

  return p;
}

int linked_list_size(linked_list * list)
{
  if(list == NULL) return 0;
  int size = 1;
  linked_list * p = list;
  while(p = p->next) ++size;
  return size;
}

TEST_CASE(test_linked_list_append)
{
  ASSERT_INT_EQAUL(0, linked_list_size(NULL));

  int array[] = { 1, 2, 3, 4, 5 };
  linked_list * root = linked_list_allocate(NULL);
  ASSERT_INT_EQAUL(1, linked_list_size(root));

  linked_list * tmp = root;
  for(int i = 0; i < 5; ++i)
  {
    int * p = array + i;
    tmp = linked_list_append(tmp, (void*)p);
    ASSERT_INT_EQAUL(*p, *(int*)(tmp->data));
    ASSERT_NULL(tmp->next);
    ASSERT_INT_EQAUL(*p, *(int*)linked_list_nth(root, i + 1)->data);
    ASSERT_NULL(linked_list_nth(root, i + 1)->next);
  }

  ASSERT_INT_EQAUL(6, linked_list_size(root));
}
