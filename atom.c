/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include <string.h>
#include <stdlib.h>
#include "atom.h"
#include "object_func.h"
#include "symbol.h"
#include "stringutils.h"
#include "unittest.h"

lisp_object * lisp_object_atom()
{
  lisp_object * atom = (lisp_object*)malloc(sizeof(lisp_object));
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

int lisp_object_parse_atom(const char * str, lisp_object ** atom)
{
  const char * p = str;
  p += skip_chars_while(is_white_space_char, p);
  const char * beg = p;
  while(!is_white_space_char(*p) && 
        !is_end_string_char(*p) && 
        !is_begin_list_char(*p) && 
        !is_end_list_char(*p))
  {
    ++p;
  }
  const char * end = p;

  char * name = (char*)malloc(sizeof(char) * (end - beg + 1));
  {
    char * p = name;
    while(beg != end)
    {
      *p = *beg;
      ++p;
      ++beg;
    }
    *p = 0;
  }

  *atom = lisp_object_atom();
  if(strlen(name) > 0)
  {
    if(is_number_string(name))
    {
      int * p = (int*)malloc(sizeof(int));
      *p      = atoi(name);

      (*atom)->atom     = p;
      (*atom)->type     = LISP_OBJECT_ATOM;
      (*atom)->sub_type = LISP_OBJECT_ATOM_INTEGER;
    }
    else
    {
      lisp_symbol * sym = lisp_symbol_allocate();
      sym->name         = name;
      (*atom)->atom     = sym;
      (*atom)->type     = LISP_OBJECT_ATOM;
      (*atom)->sub_type = LISP_OBJECT_ATOM_SYMBOL;
    }
  }
  else
  {
    (*atom)->atom     = lisp_symbol_nil();
    (*atom)->type     = LISP_OBJECT_ATOM;
    (*atom)->sub_type = LISP_OBJECT_ATOM_SYMBOL;
  }

  return end - str;
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
    lisp_object_free(atom);
  }

  {
    lisp_object * atom = lisp_object_atom_with_string("");
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM, atom->type);
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM_STRING, atom->sub_type);
    ASSERT_STRING_EQUAL("", (char*)atom->atom);
    lisp_object_free(atom);
  }

  {
    lisp_object * atom = lisp_object_atom_with_string(NULL);
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM, atom->type);
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM_STRING, atom->sub_type);
    ASSERT_NULL(atom->atom);
    lisp_object_free(atom);
  }
}

TEST_CASE(test_lisp_object_parse_atom)
{
  {
    lisp_object * atom;
    ASSERT_INT_EQUAL(0, lisp_object_parse_atom("", &atom));
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM, atom->type);
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM_SYMBOL, atom->sub_type);
    ASSERT_STRING_EQUAL("nil", get_symbol_name(atom));
    lisp_object_free(atom);
  }

  {
    lisp_object * atom;
    ASSERT_INT_EQUAL(1, lisp_object_parse_atom("a", &atom));
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM, atom->type);
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM_SYMBOL, atom->sub_type);
    ASSERT_STRING_EQUAL("a", get_symbol_name(atom));
    lisp_object_free(atom);
  }

  {
    lisp_object * atom;
    ASSERT_INT_EQUAL(3, lisp_object_parse_atom(" a1", &atom));
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM, atom->type);
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM_SYMBOL, atom->sub_type);
    ASSERT_STRING_EQUAL("a1", get_symbol_name(atom));
    lisp_object_free(atom);
  }

  {
    lisp_object * atom;
    ASSERT_INT_EQUAL(3, lisp_object_parse_atom(" a1 b2", &atom));
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM, atom->type);
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM_SYMBOL, atom->sub_type);
    ASSERT_STRING_EQUAL("a1", get_symbol_name(atom));
    lisp_object_free(atom);
  }

  {
    lisp_object * atom;
    ASSERT_INT_EQUAL(3, lisp_object_parse_atom(" a1(", &atom));
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM, atom->type);
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM_SYMBOL, atom->sub_type);
    ASSERT_STRING_EQUAL("a1", get_symbol_name(atom));
    lisp_object_free(atom);
  }

  {
    lisp_object * atom;
    ASSERT_INT_EQUAL(3, lisp_object_parse_atom(" a1)", &atom));
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM, atom->type);
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM_SYMBOL, atom->sub_type);
    ASSERT_STRING_EQUAL("a1", get_symbol_name(atom));
    lisp_object_free(atom);
  }

  {
    lisp_object * atom;
    ASSERT_INT_EQUAL(0, lisp_object_parse_atom("(", &atom));
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM, atom->type);
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM_SYMBOL, atom->sub_type);
    ASSERT_STRING_EQUAL("nil", get_symbol_name(atom));
    lisp_object_free(atom);
  }

  {
    lisp_object * atom;
    ASSERT_INT_EQUAL(1, lisp_object_parse_atom(" (", &atom));
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM, atom->type);
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM_SYMBOL, atom->sub_type);
    ASSERT_STRING_EQUAL("nil", get_symbol_name(atom));
    lisp_object_free(atom);
  }
}
