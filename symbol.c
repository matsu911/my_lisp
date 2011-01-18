/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include "symbol.h"
#include <stdlib.h>
#include "stringutils.h"
#include "unittest.h"

symbol LISP_SYMBOL_NIL = { 0, "nil" };

symbol * symbol_nil()
{
  return &LISP_SYMBOL_NIL;
}

symbol * new_symbol()
{
  symbol * p = (symbol*)malloc(sizeof(symbol));
  
  return p;
}

void delete_symbol(symbol * sym)
{
  if(sym == NULL || is_symbol_nil(sym)) return;

  if(sym->name)
    free(sym->name);

  free(sym);
}

boolean is_symbol_nil(const symbol * sym)
{
  if(sym == &LISP_SYMBOL_NIL)
    return TRUE;
  else
    return FALSE;
}

TEST_CASE(test_is_symbol_nil)
{
  ASSERT_TRUE(is_symbol_nil(symbol_nil()));
}
