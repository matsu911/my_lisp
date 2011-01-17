/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef __OBJECT_FUNC_H__
#define __OBJECT_FUNC_H__

#include "object.h"
#include "atom.h"
#include "cons.h"

Object * new_Object();
Object * new_Object_with_Atom(Atom * atom);
Object * new_Object_with_Cons(Cons * cons);
void delete_Object(Object * object);

#endif /* __OBJECT_FUNC_H__ */
