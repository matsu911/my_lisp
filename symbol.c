/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include "symbol.h"
#include <stdlib.h>
#include "stringutils.h"

Symbol * new_symbol()
{
  Symbol * p = (Symbol*)malloc(sizeof(Symbol));
  
  return p;
}

Symbol * new_symbol_nil()
{
  Symbol * p = new_symbol();
  p->id = 0;
  p->name = allocate_string("nil");
  return p;
}

void delete_symbol(Symbol * symbol)
{
  if(symbol == NULL) return;

  if(symbol->name)
    free(symbol->name);

  free(symbol);
}
