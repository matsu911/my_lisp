/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef __CONS_H__
#define __CONS_H__

#include "object.h"
#include "atom.h"

typedef struct _Cons
{
  Object * car;
  Object * cdr;
} Cons;

/* Object * new_Object_with_Cons(Cons * cons); */

#endif /* __CONS_H__ */
