/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef __CONS_FUNC_H__
#define __CONS_FUNC_H__

#include "atom.h"

lisp_object * lisp_object_cons(lisp_object * car, lisp_object * cdr);

int lisp_object_parse_cons(const char * str, lisp_object ** cons);

void lisp_object_print_cons(const lisp_object * cons);

#endif /* __CONS_FUNC_H__ */
