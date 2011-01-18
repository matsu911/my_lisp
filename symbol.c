/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include "symbol.h"
#include <stdlib.h>
#include "stringutils.h"
#include "unittest.h"

lisp_symbol LISP_SYMBOL_NIL = { 0, "nil" };

lisp_symbol * lisp_symbol_nil()
{
  return &LISP_SYMBOL_NIL;
}

lisp_symbol * lisp_symbol_allocate()
{
  lisp_symbol * p = (lisp_symbol*)malloc(sizeof(lisp_symbol));
  
  return p;
}

void lisp_symbol_free(lisp_symbol * sym)
{
  if(sym == NULL || is_lisp_symbol_nil(sym)) return;

  if(sym->name)
    free(sym->name);

  free(sym);
}

boolean is_lisp_symbol_nil(const lisp_symbol * sym)
{
  if(sym == &LISP_SYMBOL_NIL)
    return TRUE;
  else
    return FALSE;
}

TEST_CASE(test_is_lisp_symbol_nil)
{
  ASSERT_TRUE(is_lisp_symbol_nil(lisp_symbol_nil()));
}
