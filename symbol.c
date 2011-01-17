/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include "symbol.h"
#include <stdlib.h>
#include "stringutils.h"
#include "unittest.h"

Symbol LISP_SYMBOL_NIL = { 0, "nil" };

Symbol * symbol_nil()
{
  return &LISP_SYMBOL_NIL;
}

Symbol * new_symbol()
{
  Symbol * p = (Symbol*)malloc(sizeof(Symbol));
  
  return p;
}

void delete_symbol(Symbol * symbol)
{
  if(symbol == NULL || is_symbol_nil(symbol)) return;

  if(symbol->name)
    free(symbol->name);

  free(symbol);
}

LISP_BOOL is_symbol_nil(const Symbol * symbol)
{
  if(symbol == &LISP_SYMBOL_NIL)
    return LISP_TRUE;
  else
    return LISP_FALSE;
}

TEST_CASE(test_is_symbol_nil)
{
  ASSERT_TRUE(is_symbol_nil(symbol_nil()));
}
