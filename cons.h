/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef __CONS_H__
#define __CONS_H__

#include "atom.h"

enum OBJECT_TYPE
{
  OBJECT_ATOM,
  OBJECT_CONS,
};

typedef struct _Object
{
  enum OBJECT_TYPE type;
  void * ptr;
} Object;

Object * new_Object();
Object * new_Object_with_Atom(Atom * atom);
void delete_Object(Object * object);

typedef struct _Cons
{
  Object * car;
  Object * cdr;
} Cons;

Cons * new_Cons();

void delete_Cons(Cons * cons);

int parse_Cons(const char * str, Cons ** cons);

#endif /* __CONS_H__ */
