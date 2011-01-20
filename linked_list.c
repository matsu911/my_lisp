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
  linked_list * prev = NULL;
  linked_list * tmp  = list;

  while(tmp)
  {
    if(tmp == p)
    {
      linked_list * next = tmp->next;
      if(prev)
        prev->next = next;
      linked_list_free(tmp);
      return next;
    }
      
    prev = tmp;
    tmp  = tmp->next;
  }

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
  while((p = p->next) != NULL) ++size;
  return size;
}

void linked_list_traverse(linked_list * list, void (*func)(linked_list *))
{
  if(list == NULL) return;

  linked_list * p = list;
  while(p)
  {
    linked_list * next = p->next;
    func(p);
    p = next;
  }
}

void linked_list_traverse_free(linked_list * list)
{
  linked_list_traverse(list, linked_list_free);
}

linked_list * linked_list_find(linked_list * list, void * val, boolean (*matcher)(void*, void*))
{
  if(list == NULL) return NULL;

  linked_list * p = list;
  while(p)
  {
    if(matcher(p->data, val))
      return p;
    p = p->next;
  }

  return NULL;
}

static boolean match_int(void * a, void * b)
{
  return a && b && *(int*)a == *(int*)b;
}

/* static void print_int(linked_list * list) */
/* { */
/*   if(list->data) */
/*     printf("%d ", *(int*)list->data); */
/* } */

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
  /* linked_list_traverse(root, print_int); */
  {
    linked_list * p = linked_list_find(root, array + 2, match_int);
    ASSERT_NOT_NULL(p);
    ASSERT_INT_EQAUL(3, *(int*)p->data);
  }

  ASSERT_NOT_NULL(root = linked_list_delete(root, root));

  ASSERT_INT_EQAUL(5, linked_list_size(root));
  /* linked_list_traverse(root, print_int); */
  {
    linked_list * p = linked_list_find(root, array + 2, match_int);
    ASSERT_NOT_NULL(p);
    ASSERT_INT_EQAUL(3, *(int*)p->data);
  }

  ASSERT_NOT_NULL(root = linked_list_delete(root, root));

  ASSERT_INT_EQAUL(4, linked_list_size(root));
  /* linked_list_traverse(root, print_int); */
  {
    linked_list * p = linked_list_find(root, array + 2, match_int);
    ASSERT_NOT_NULL(p);
    ASSERT_INT_EQAUL(3, *(int*)p->data);
  }

  ASSERT_NOT_NULL(linked_list_delete(root, root->next));

  ASSERT_INT_EQAUL(3, linked_list_size(root));
  /* linked_list_traverse(root, print_int); */
  ASSERT_NULL(linked_list_find(root, array + 2, match_int));

  ASSERT_NULL(linked_list_delete(root, ((linked_list*)((linked_list*)root->next)->next)->next));

  ASSERT_INT_EQAUL(3, linked_list_size(root));
  /* linked_list_traverse(root, print_int); */

  ASSERT_NULL(linked_list_delete(root, ((linked_list*)root->next)->next));

  ASSERT_INT_EQAUL(2, linked_list_size(root));
  /* linked_list_traverse(root, print_int); */

  ASSERT_NULL(linked_list_delete(root, root->next));

  ASSERT_INT_EQAUL(1, linked_list_size(root));
  /* linked_list_traverse(root, print_int); */

  ASSERT_NULL(root = linked_list_delete(root, root));

  ASSERT_INT_EQAUL(0, linked_list_size(root));
  /* linked_list_traverse(root, print_int); */
  
  linked_list_traverse_free(root);
}
