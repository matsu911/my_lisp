/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include <string.h>
#include <stdlib.h>
#include "atom.h"
#include "symbol.h"
#include "stringutils.h"
#include "unittest.h"

const char * get_symbol_name(const Atom * atom)
{
  if(atom == NULL) return NULL;

  if(atom->type == ATOM_SYMBOL && atom->ptr)
    return ((Symbol*)atom->ptr)->name;

  return NULL;
}

Atom * new_Atom()
{
  Atom * atom = (Atom*)malloc(sizeof(Atom));
  atom->ptr = NULL;
  return atom;
}

void delete_Atom(Atom * atom)
{
  if(atom->ptr == NULL )
    return;

  if(atom->type == ATOM_SYMBOL)
    delete_symbol(atom->ptr);
}

int parse_Atom(const char * str, Atom ** atom)
{
  const char * p = str;
  while(is_white_space_char(*p) && !is_end_string_char(*p)) ++p;
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
  Symbol * symbol = new_symbol();
  symbol->name = name;
  (*atom)->ptr = symbol;
  (*atom)->type = ATOM_SYMBOL;

  return end - str;
}

TEST_CASE(test_parse_Atom)
{
  {
    Atom * atom;
    ASSERT_INT_EQAUL(0, parse_Atom("", &atom));
    ASSERT_STRING_EQUAL("", get_symbol_name(atom));
    delete_Atom(atom);
  }

  {
    Atom * atom;
    ASSERT_INT_EQAUL(1, parse_Atom("a", &atom));
    ASSERT_STRING_EQUAL("a", get_symbol_name(atom));
    delete_Atom(atom);
  }

  {
    Atom * atom;
    ASSERT_INT_EQAUL(3, parse_Atom(" a1", &atom));
    ASSERT_STRING_EQUAL("a1", get_symbol_name(atom));
    delete_Atom(atom);
  }

  {
    Atom * atom;
    ASSERT_INT_EQAUL(3, parse_Atom(" a1 b2", &atom));
    ASSERT_STRING_EQUAL("a1", get_symbol_name(atom));
    delete_Atom(atom);
  }

  {
    Atom * atom;
    ASSERT_INT_EQAUL(3, parse_Atom(" a1(", &atom));
    ASSERT_STRING_EQUAL("a1", get_symbol_name(atom));
    delete_Atom(atom);
  }

  {
    Atom * atom;
    ASSERT_INT_EQAUL(3, parse_Atom(" a1)", &atom));
    ASSERT_STRING_EQUAL("a1", get_symbol_name(atom));
    delete_Atom(atom);
  }

  {
    Atom * atom;
    ASSERT_INT_EQAUL(0, parse_Atom("(", &atom));
    ASSERT_STRING_EQUAL("", get_symbol_name(atom));
    delete_Atom(atom);
  }

  {
    Atom * atom;
    ASSERT_INT_EQAUL(1, parse_Atom(" (", &atom));
    ASSERT_STRING_EQUAL("", get_symbol_name(atom));
    delete_Atom(atom);
  }
}
