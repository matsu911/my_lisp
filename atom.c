/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include <string.h>
#include <stdlib.h>
#include "atom.h"
#include "object_func.h"
#include "symbol.h"
#include "stringutils.h"
#include "unittest.h"

lisp_object * new_Atom()
{
  lisp_object * atom = (lisp_object*)malloc(sizeof(lisp_object));
  atom->atom = NULL;
  atom->type = LISP_OBJECT_ATOM;
  return atom;
}

lisp_object * new_Atom_with_symbol(symbol * sym)
{
  lisp_object * atom  = new_Atom();
  atom->type     = LISP_OBJECT_ATOM;
  atom->sub_type = LISP_OBJECT_ATOM_SYMBOL;
  atom->atom     = (void*)sym;
  return atom;
}

lisp_object * new_Atom_with_string(const char * str)
{
  lisp_object * atom  = new_Atom();
  atom->type     = LISP_OBJECT_ATOM;
  atom->sub_type = LISP_OBJECT_ATOM_STRING;
  atom->atom     = (void*)allocate_string(str);
  return atom;
}

int parse_Atom(const char * str, lisp_object ** atom)
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

  *atom = new_Atom();
  if(strlen(name) > 0)
  {
    symbol * sym      = new_symbol();
    sym->name         = name;
    (*atom)->atom     = sym;
    (*atom)->type     = LISP_OBJECT_ATOM;
    (*atom)->sub_type = LISP_OBJECT_ATOM_SYMBOL;
  }
  else
  {
    (*atom)->atom     = symbol_nil();
    (*atom)->type     = LISP_OBJECT_ATOM;
    (*atom)->sub_type = LISP_OBJECT_ATOM_SYMBOL;
  }

  return end - str;
}

const char * get_symbol_name(const lisp_object * atom)
{
  if(atom == NULL) return NULL;

  if(atom->type == LISP_OBJECT_ATOM && atom->sub_type == LISP_OBJECT_ATOM_SYMBOL && atom->atom)
    return ((symbol*)atom->atom)->name;

  return NULL;
}

TEST_CASE(test_new_Atom_with_string)
{
  {
    lisp_object * atom = new_Atom_with_string("abc");
    ASSERT_INT_EQAUL(LISP_OBJECT_ATOM, atom->type);
    ASSERT_INT_EQAUL(LISP_OBJECT_ATOM_STRING, atom->sub_type);
    ASSERT_STRING_EQUAL("abc", (char*)atom->atom);
    lisp_object_delete(atom);
  }

  {
    lisp_object * atom = new_Atom_with_string("");
    ASSERT_INT_EQAUL(LISP_OBJECT_ATOM, atom->type);
    ASSERT_INT_EQAUL(LISP_OBJECT_ATOM_STRING, atom->sub_type);
    ASSERT_STRING_EQUAL("", (char*)atom->atom);
    lisp_object_delete(atom);
  }

  {
    lisp_object * atom = new_Atom_with_string(NULL);
    ASSERT_INT_EQAUL(LISP_OBJECT_ATOM, atom->type);
    ASSERT_INT_EQAUL(LISP_OBJECT_ATOM_STRING, atom->sub_type);
    ASSERT_NULL(atom->atom);
    lisp_object_delete(atom);
  }
}

TEST_CASE(test_parse_Atom)
{
  {
    lisp_object * atom;
    ASSERT_INT_EQAUL(0, parse_Atom("", &atom));
    ASSERT_INT_EQAUL(LISP_OBJECT_ATOM, atom->type);
    ASSERT_INT_EQAUL(LISP_OBJECT_ATOM_SYMBOL, atom->sub_type);
    ASSERT_STRING_EQUAL("nil", get_symbol_name(atom));
    lisp_object_delete(atom);
  }

  {
    lisp_object * atom;
    ASSERT_INT_EQAUL(1, parse_Atom("a", &atom));
    ASSERT_INT_EQAUL(LISP_OBJECT_ATOM, atom->type);
    ASSERT_INT_EQAUL(LISP_OBJECT_ATOM_SYMBOL, atom->sub_type);
    ASSERT_STRING_EQUAL("a", get_symbol_name(atom));
    lisp_object_delete(atom);
  }

  {
    lisp_object * atom;
    ASSERT_INT_EQAUL(3, parse_Atom(" a1", &atom));
    ASSERT_INT_EQAUL(LISP_OBJECT_ATOM, atom->type);
    ASSERT_INT_EQAUL(LISP_OBJECT_ATOM_SYMBOL, atom->sub_type);
    ASSERT_STRING_EQUAL("a1", get_symbol_name(atom));
    lisp_object_delete(atom);
  }

  {
    lisp_object * atom;
    ASSERT_INT_EQAUL(3, parse_Atom(" a1 b2", &atom));
    ASSERT_INT_EQAUL(LISP_OBJECT_ATOM, atom->type);
    ASSERT_INT_EQAUL(LISP_OBJECT_ATOM_SYMBOL, atom->sub_type);
    ASSERT_STRING_EQUAL("a1", get_symbol_name(atom));
    lisp_object_delete(atom);
  }

  {
    lisp_object * atom;
    ASSERT_INT_EQAUL(3, parse_Atom(" a1(", &atom));
    ASSERT_INT_EQAUL(LISP_OBJECT_ATOM, atom->type);
    ASSERT_INT_EQAUL(LISP_OBJECT_ATOM_SYMBOL, atom->sub_type);
    ASSERT_STRING_EQUAL("a1", get_symbol_name(atom));
    lisp_object_delete(atom);
  }

  {
    lisp_object * atom;
    ASSERT_INT_EQAUL(3, parse_Atom(" a1)", &atom));
    ASSERT_INT_EQAUL(LISP_OBJECT_ATOM, atom->type);
    ASSERT_INT_EQAUL(LISP_OBJECT_ATOM_SYMBOL, atom->sub_type);
    ASSERT_STRING_EQUAL("a1", get_symbol_name(atom));
    lisp_object_delete(atom);
  }

  {
    lisp_object * atom;
    ASSERT_INT_EQAUL(0, parse_Atom("(", &atom));
    ASSERT_INT_EQAUL(LISP_OBJECT_ATOM, atom->type);
    ASSERT_INT_EQAUL(LISP_OBJECT_ATOM_SYMBOL, atom->sub_type);
    ASSERT_STRING_EQUAL("nil", get_symbol_name(atom));
    lisp_object_delete(atom);
  }

  {
    lisp_object * atom;
    ASSERT_INT_EQAUL(1, parse_Atom(" (", &atom));
    ASSERT_INT_EQAUL(LISP_OBJECT_ATOM, atom->type);
    ASSERT_INT_EQAUL(LISP_OBJECT_ATOM_SYMBOL, atom->sub_type);
    ASSERT_STRING_EQUAL("nil", get_symbol_name(atom));
    lisp_object_delete(atom);
  }
}
