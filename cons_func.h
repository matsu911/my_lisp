/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef __CONS_FUNC_H__
#define __CONS_FUNC_H__

#include "cons.h"
#include "atom.h"

Cons * new_Cons();
Cons * new_Cons_with_Atom_Atom(Atom * car, Atom * cdr);

void delete_Cons(Cons * cons);

int parse_Cons(const char * str, Cons ** cons);

#endif /* __CONS_FUNC_H__ */
