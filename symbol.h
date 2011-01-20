/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include "type.h"

typedef struct _lisp_symbol
{
  int id;
  char * name;
} lisp_symbol;

lisp_symbol * lisp_symbol_allocate();
lisp_symbol * lisp_symbol_nil();
void lisp_symbol_free(lisp_symbol * sym);

boolean is_lisp_symbol_nil(const lisp_symbol * sym);

lisp_symbol * lisp_symbol_make(const char * name);
lisp_symbol * lisp_symbol_lookup(const char * name);

#endif /* __SYMBOL_H__ */
