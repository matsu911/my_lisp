/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include "type.h"

typedef struct _symbol
{
  int id;
  char * name;
} symbol;

symbol * new_symbol();
symbol * symbol_nil();
void delete_symbol(symbol * sym);

boolean is_symbol_nil(const symbol * sym);

#endif /* __SYMBOL_H__ */
