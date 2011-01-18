/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef __SYMBOL_H__
#define __SYMBOL_H__

#include "type.h"

typedef struct _Symbol
{
  int id;
  char * name;
} Symbol;

Symbol * new_symbol();
Symbol * symbol_nil();
void delete_symbol(Symbol * symbol);

boolean is_symbol_nil(const Symbol * symbol);

#endif /* __SYMBOL_H__ */
