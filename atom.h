/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef __ATOM_H__
#define __ATOM_H__

#include "symbol.h"

typedef struct _Atom
{
  enum { ATOM_INTEGER,
         ATOM_SYMBOL,
  } type;

  void * ptr;
} Atom;

int parse_Atom(const char * str, Atom ** atom);

void delete_Atom(Atom * atom);

Atom * new_Atom();
Atom * new_Atom_with_Symbol(Symbol * symbol);

const char * get_symbol_name(const Atom * atom);

#endif /* __ATOM_H__ */
