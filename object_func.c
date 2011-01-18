/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include <stdlib.h>
#include "object_func.h"
#include "cons_func.h"

void my_free(void* p)
{
  /* printf("%p\n", p); */
  free(p);
}

void lisp_object_free(lisp_object * object)
{
  if(object == NULL) return;

  if(object->type == LISP_OBJECT_ATOM)
  {
    if(object->atom != NULL && object->sub_type == LISP_OBJECT_ATOM_SYMBOL)
      lisp_symbol_free((lisp_symbol*)object->atom);
  }
  else if(object->type == LISP_OBJECT_CONS)
  {
    if(object->car != NULL)
    {
      lisp_object_free(object->car);
    }

    if(object->cdr != NULL)
    {
      lisp_object_free(object->cdr);
    }
  }

  my_free((void*)object);
}
