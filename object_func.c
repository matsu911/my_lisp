/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include <stdlib.h>
#include "object_func.h"
#include "cons_func.h"

Object * new_Object()
{
  Object * p = (Object *)malloc(sizeof(Object));
  /* if(p == NULL) */
  /*   xxxxx; */
  return p;
}

Object * new_Object_with_Atom(Atom * atom)
{
  Object * p = new_Object();
  p->ptr = (void*)atom;
  p->type = OBJECT_ATOM;
  return p;
}

Object * new_Object_with_Cons(Cons * cons)
{
  Object * p = new_Object();
  p->ptr = (void*)cons;
  p->type = OBJECT_CONS;
  return p;
}

void delete_Object(Object * object)
{
  if(object == NULL || object->ptr == NULL) return;

  if(object->type == OBJECT_ATOM)
    delete_Atom(object->ptr);
  else if(object->type == OBJECT_CONS)
    delete_Cons(object->ptr);
}
