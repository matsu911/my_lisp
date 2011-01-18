/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef __ATOM_H__
#define __ATOM_H__

#include "symbol.h"
#include "object.h"

int parse_Atom(const char * str, lisp_object ** atom);

lisp_object * new_Atom();
lisp_object * new_Atom_with_symbol(symbol * sym);
lisp_object * new_Atom_with_string(const char * str);

const char * get_symbol_name(const lisp_object * atom);

#endif /* __ATOM_H__ */
