/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef __CONS_FUNC_H__
#define __CONS_FUNC_H__

#include "atom.h"

lisp_object * new_Cons(lisp_object * car, lisp_object * cdr);

int parse_Cons(const char * str, lisp_object ** cons);

void print_Cons(const lisp_object * cons);

#endif /* __CONS_FUNC_H__ */
