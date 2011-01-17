/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef __ATOM_H__
#define __ATOM_H__

#include "symbol.h"
#include "object.h"

int parse_Atom(const char * str, Object ** atom);

Object * new_Atom();
Object * new_Atom_with_Symbol(Symbol * symbol);

const char * get_symbol_name(const Object * atom);

#endif /* __ATOM_H__ */
