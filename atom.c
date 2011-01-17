/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include <string.h>
#include <stdlib.h>
#include "atom.h"
#include "object_func.h"
#include "symbol.h"
#include "stringutils.h"
#include "unittest.h"

Object * new_Atom()
{
  Object * atom = (Object*)malloc(sizeof(Object));
  atom->atom = NULL;
  atom->type = OBJECT_ATOM;
  return atom;
}

Object * new_Atom_with_Symbol(Symbol * symbol)
{
  Object * atom = new_Atom();
  atom->type = OBJECT_ATOM;
  atom->sub_type = OBJECT_ATOM_SYMBOL;
  atom->atom = (void*)symbol;
  return atom;
}

int parse_Atom(const char * str, Object ** atom)
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
    Symbol * symbol   = new_symbol();
    symbol->name      = name;
    (*atom)->atom     = symbol;
    (*atom)->type     = OBJECT_ATOM;
    (*atom)->sub_type = OBJECT_ATOM_SYMBOL;
  }
  else
  {
    (*atom)->atom     = symbol_nil();
    (*atom)->type     = OBJECT_ATOM;
    (*atom)->sub_type = OBJECT_ATOM_SYMBOL;
  }

  return end - str;
}

const char * get_symbol_name(const Object * atom)
{
  if(atom == NULL) return NULL;

  if(atom->type == OBJECT_ATOM && atom->sub_type == OBJECT_ATOM_SYMBOL && atom->atom)
    return ((Symbol*)atom->atom)->name;

  return NULL;
}

TEST_CASE(test_parse_Atom)
{
  {
    Object * atom;
    ASSERT_INT_EQAUL(0, parse_Atom("", &atom));
    ASSERT_INT_EQAUL(OBJECT_ATOM, atom->type);
    ASSERT_INT_EQAUL(OBJECT_ATOM_SYMBOL, atom->sub_type);
    ASSERT_STRING_EQUAL("nil", get_symbol_name(atom));
    delete_Object(atom);
  }

  {
    Object * atom;
    ASSERT_INT_EQAUL(1, parse_Atom("a", &atom));
    ASSERT_INT_EQAUL(OBJECT_ATOM, atom->type);
    ASSERT_INT_EQAUL(OBJECT_ATOM_SYMBOL, atom->sub_type);
    ASSERT_STRING_EQUAL("a", get_symbol_name(atom));
    delete_Object(atom);
  }

  {
    Object * atom;
    ASSERT_INT_EQAUL(3, parse_Atom(" a1", &atom));
    ASSERT_INT_EQAUL(OBJECT_ATOM, atom->type);
    ASSERT_INT_EQAUL(OBJECT_ATOM_SYMBOL, atom->sub_type);
    ASSERT_STRING_EQUAL("a1", get_symbol_name(atom));
    delete_Object(atom);
  }

  {
    Object * atom;
    ASSERT_INT_EQAUL(3, parse_Atom(" a1 b2", &atom));
    ASSERT_INT_EQAUL(OBJECT_ATOM, atom->type);
    ASSERT_INT_EQAUL(OBJECT_ATOM_SYMBOL, atom->sub_type);
    ASSERT_STRING_EQUAL("a1", get_symbol_name(atom));
    delete_Object(atom);
  }

  {
    Object * atom;
    ASSERT_INT_EQAUL(3, parse_Atom(" a1(", &atom));
    ASSERT_INT_EQAUL(OBJECT_ATOM, atom->type);
    ASSERT_INT_EQAUL(OBJECT_ATOM_SYMBOL, atom->sub_type);
    ASSERT_STRING_EQUAL("a1", get_symbol_name(atom));
    delete_Object(atom);
  }

  {
    Object * atom;
    ASSERT_INT_EQAUL(3, parse_Atom(" a1)", &atom));
    ASSERT_INT_EQAUL(OBJECT_ATOM, atom->type);
    ASSERT_INT_EQAUL(OBJECT_ATOM_SYMBOL, atom->sub_type);
    ASSERT_STRING_EQUAL("a1", get_symbol_name(atom));
    delete_Object(atom);
  }

  {
    Object * atom;
    ASSERT_INT_EQAUL(0, parse_Atom("(", &atom));
    ASSERT_INT_EQAUL(OBJECT_ATOM, atom->type);
    ASSERT_INT_EQAUL(OBJECT_ATOM_SYMBOL, atom->sub_type);
    ASSERT_STRING_EQUAL("nil", get_symbol_name(atom));
    delete_Object(atom);
  }

  {
    Object * atom;
    ASSERT_INT_EQAUL(1, parse_Atom(" (", &atom));
    ASSERT_INT_EQAUL(OBJECT_ATOM, atom->type);
    ASSERT_INT_EQAUL(OBJECT_ATOM_SYMBOL, atom->sub_type);
    ASSERT_STRING_EQUAL("nil", get_symbol_name(atom));
    delete_Object(atom);
  }
}
