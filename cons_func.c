/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include "cons_func.h"
#include "object_func.h"
#include "symbol.h"
#include "unittest.h"
#include "stringutils.h"

Cons * allocate_Cons()
{
  Cons * cons = (Cons*)malloc(sizeof(Cons));
  return cons;
}

Cons * new_Cons()
{
  return new_Cons_with_Atom_Atom(new_Atom_with_Symbol(new_symbol_nil()),
                                 new_Atom_with_Symbol(new_symbol_nil()));
}

Cons * new_Cons_with_Atom_Atom(Atom * car, Atom * cdr)
{
  Cons * cons = allocate_Cons();
  cons->car = new_Object_with_Atom(car);
  cons->cdr = new_Object_with_Atom(cdr);
  return cons;
}

Cons * new_Cons_with_Atom_Cons(Atom * car, Cons * cdr)
{
  Cons * cons = allocate_Cons();
  cons->car = new_Object_with_Atom(car);
  cons->cdr = new_Object_with_Cons(cdr);
  return cons;
}

void delete_Cons(Cons * cons)
{
  delete_Object(cons->car);
  delete_Object(cons->cdr);
}

int parse_Cons_internal(const char * str, Cons ** cons)
{
  const char * p = str;

  if(is_end_string_char(*p))
  {
    *cons = NULL;
    return p - str;
  }

  if(is_begin_list_char(*p)) ++p;

  *cons = new_Cons();

  // car part
  if(is_begin_list_char(*p))
  {
    Cons * c;
    p += parse_Cons(p, &c);
  }
  else
  {
    Atom * atom;
    p += parse_Atom(p, &atom);
    (*cons)->car = new_Object_with_Atom(atom);
  }

  p += skip_chars_while(is_white_space_char, p);

  // cdr part
  if(is_cons_dot_char(*p))
  {
    ++p;
    Atom * atom;
    p += parse_Atom(p, &atom);
    (*cons)->cdr = new_Object_with_Atom(atom);
  }
  else if(is_end_string_char(next_char_not(is_white_space_char, p)) ||
          is_end_list_char(next_char_not(is_white_space_char, p)))
  {
    (*cons)->cdr = new_Object_with_Atom(new_Atom_with_Symbol(new_symbol_nil()));
  }
  else
  {
    Cons * tmp;
    p += parse_Cons_internal(p, &tmp);
    (*cons)->cdr = new_Object_with_Cons(tmp);
  }

  return p - str;
}

int parse_Cons(const char * str, Cons ** cons)
{
  const char * p = str;
  p += skip_chars_while_not(is_begin_list_char, p);

  p += parse_Cons_internal(p, cons);

  p += skip_chars_while_not(is_end_list_char, p);
  if(is_end_list_char(*p)) ++p;

  return p - str;
}

Atom * car_as_Atom(const Cons * cons)
{
  return (Atom*)cons->car->ptr;
}

Atom * cdr_as_Atom(const Cons * cons)
{
  return (Atom*)cons->cdr->ptr;
}

Cons * car_as_Cons(const Cons * cons)
{
  return (Cons*)cons->car->ptr;
}

Cons * cdr_as_Cons(const Cons * cons)
{
  return (Cons*)cons->cdr->ptr;
}

TEST_CASE(test_parse_Cons)
{
  {
    Cons * cons;
    const char * s = " ";
    int size = parse_Cons(s, &cons);
    ASSERT_INT_EQAUL(1, size);
    ASSERT_TRUE(cons == NULL);
  }

  {
    Cons * cons;
    const char * s = "(a . b)";
    ASSERT_INT_EQAUL(7, parse_Cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(car_as_Atom(cons)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(cdr_as_Atom(cons)));
    delete_Cons(cons);
  }

  {
    Cons * cons;
    const char * s = " (a  .  b) ";
    ASSERT_INT_EQAUL(10, parse_Cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(car_as_Atom(cons)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(cdr_as_Atom(cons)));
    delete_Cons(cons);
  }

  {
    Cons * cons;
    const char * s = " (a1b2#!a  .  b*0f1@:) ";
    ASSERT_INT_EQAUL(22, parse_Cons(s, &cons));
    ASSERT_STRING_EQUAL("a1b2#!a", get_symbol_name(car_as_Atom(cons)));
    ASSERT_STRING_EQUAL("b*0f1@:", get_symbol_name(cdr_as_Atom(cons)));
    delete_Cons(cons);
  }

  {
    Cons * cons;
    const char * s = " (a b) ";
    parse_Cons(s, &cons);
    ASSERT_INT_EQAUL(6, parse_Cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(car_as_Atom(cons)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(car_as_Atom(cdr_as_Cons(cons))));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(cdr_as_Atom(cdr_as_Cons(cons))));
    delete_Cons(cons);
  }

  {
    Cons * cons;
    const char * s = " (a b c) ";
    ASSERT_INT_EQAUL(8, parse_Cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(car_as_Atom(cons)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(car_as_Atom(cdr_as_Cons(cons))));
    ASSERT_STRING_EQUAL("c", get_symbol_name(car_as_Atom(cdr_as_Cons(cdr_as_Cons(cons)))));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(cdr_as_Atom(cdr_as_Cons(cdr_as_Cons(cons)))));
    delete_Cons(cons);
  }

}
