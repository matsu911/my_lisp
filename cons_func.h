/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef __CONS_FUNC_H__
#define __CONS_FUNC_H__

#include "atom.h"

Object * new_Cons(Object * car, Object * cdr);

int parse_Cons(const char * str, Object ** cons);

void print_Cons(const Object * cons);

#endif /* __CONS_FUNC_H__ */
