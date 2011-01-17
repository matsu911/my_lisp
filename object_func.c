/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include <stdlib.h>
#include "object_func.h"
#include "cons_func.h"

void my_free(void* p)
{
  /* printf("%p\n", p); */
  free(p);
}

void delete_Object(Object * object)
{
  if(object == NULL) return;

  if(object->type == OBJECT_ATOM)
  {
    if(object->atom != NULL && object->sub_type == OBJECT_ATOM_SYMBOL)
      delete_symbol((Symbol*)object->atom);
  }
  else if(object->type == OBJECT_CONS)
  {
    if(object->car != NULL)
    {
      delete_Object(object->car);
    }

    if(object->cdr != NULL)
    {
      delete_Object(object->cdr);
    }
  }

  my_free((void*)object);
}
