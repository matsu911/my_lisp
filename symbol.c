/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include "symbol.h"
#include <stdlib.h>
#include "stringutils.h"
#include "linked_list.h"
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

static linked_list * LISP_SYMBOL_ROOT = NULL;

static boolean symbol_name_match(void * a, void * b)
{
  return a && b && strcmp(((lisp_symbol*)a)->name, (char*)b) == 0;
}

static int LISP_SYMBOL_NEXT_ID = 0;

static int lisp_symbol_next_id()
{
  return ++LISP_SYMBOL_NEXT_ID;
}

lisp_symbol * lisp_symbol_make(const char * name)
{
  if(LISP_SYMBOL_ROOT == NULL)
    LISP_SYMBOL_ROOT = linked_list_allocate(&LISP_SYMBOL_NIL);

  lisp_symbol * p = lisp_symbol_lookup(name);
  if(p)
    return p;

  lisp_symbol * sym = lisp_symbol_allocate();
  sym->id   = lisp_symbol_next_id();
  sym->name = allocate_string(name);

  linked_list_append(LISP_SYMBOL_ROOT, sym);

  return  sym;
}

lisp_symbol * lisp_symbol_lookup(const char * name)
{
  linked_list * p = linked_list_find(LISP_SYMBOL_ROOT, (void*)name, symbol_name_match);
  if(p)
    return p->data;
  return NULL;
}

TEST_CASE(test_is_lisp_symbol_nil)
{
  ASSERT_TRUE(is_lisp_symbol_nil(lisp_symbol_nil()));

  lisp_symbol * pp = lisp_symbol_make("a");

  lisp_symbol * p = lisp_symbol_lookup("a");

  ASSERT_STRING_EQUAL("a", p->name);

  ASSERT_NULL(lisp_symbol_lookup("b"));

}
