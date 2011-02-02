/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef __ATOM_H__
#define __ATOM_H__

#include "symbol.h"
#include "object.h"

lisp_object * lisp_object_atom();
lisp_object * lisp_object_atom_with_lisp_symbol(lisp_symbol * sym);
lisp_object * lisp_object_atom_with_string(const char * str);

const char * get_symbol_name(const lisp_object * atom);

#endif /* __ATOM_H__ */
