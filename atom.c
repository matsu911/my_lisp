/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include <string.h>
#include <stdlib.h>
#include <gc.h>
#include "atom.h"
#include "object_func.h"
#include "symbol.h"
#include "stringutils.h"
#include "unittest.h"

lisp_object * lisp_object_atom()
{
  lisp_object * atom = (lisp_object*)GC_MALLOC_ATOMIC(sizeof(lisp_object));
  atom->atom = NULL;
  atom->type = LISP_OBJECT_ATOM;
  return atom;
}

lisp_object * lisp_object_atom_with_lisp_symbol(lisp_symbol * sym)
{
  lisp_object * atom  = lisp_object_atom();
  atom->type     = LISP_OBJECT_ATOM;
  atom->sub_type = LISP_OBJECT_ATOM_SYMBOL;
  atom->atom     = (void*)sym;
  return atom;
}

lisp_object * lisp_object_atom_with_string(const char * str)
{
  lisp_object * atom  = lisp_object_atom();
  atom->type     = LISP_OBJECT_ATOM;
  atom->sub_type = LISP_OBJECT_ATOM_STRING;
  atom->atom     = (void*)allocate_string(str);
  return atom;
}

const char * get_symbol_name(const lisp_object * atom)
{
  if(atom == NULL) return NULL;

  if(atom->type == LISP_OBJECT_ATOM && atom->sub_type == LISP_OBJECT_ATOM_SYMBOL && atom->atom)
    return ((lisp_symbol*)atom->atom)->name;

  return NULL;
}

TEST_CASE(test_lisp_object_atom_with_string)
{
  {
    lisp_object * atom = lisp_object_atom_with_string("abc");
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM, atom->type);
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM_STRING, atom->sub_type);
    ASSERT_STRING_EQUAL("abc", (char*)atom->atom);
    /* lisp_object_free(atom); */
  }

  {
    lisp_object * atom = lisp_object_atom_with_string("");
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM, atom->type);
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM_STRING, atom->sub_type);
    ASSERT_STRING_EQUAL("", (char*)atom->atom);
    /* lisp_object_free(atom); */
  }

  {
    lisp_object * atom = lisp_object_atom_with_string(NULL);
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM, atom->type);
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM_STRING, atom->sub_type);
    ASSERT_NULL(atom->atom);
    /* lisp_object_free(atom); */
  }
}

