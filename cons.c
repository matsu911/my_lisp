/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include "cons.h"
#include "symbol.h"
#include "atom.h"
#include "unittest.h"
#include "stringutils.h"

Object * new_Object()
{
  Object * p = (Object *)malloc(sizeof(Object));
  /* if(p == NULL) */
  /*   xxxxx; */
  return p;
}

Object * new_Object_with_Atom(Atom * atom)
{
  Object * p = new_Object();
  p->ptr = (void*)atom;
  return p;
}

void delete_Object(Object * object)
{
  if(object == NULL || object->ptr == NULL) return;

  if(object->type == OBJECT_ATOM)
    delete_Atom(object->ptr);
  else if(object->type == OBJECT_CONS)
    delete_Cons(object->ptr);
}

Cons * new_Cons()
{
  Cons * cons = (Cons*)malloc(sizeof(Cons));
  cons->car = new_Object_with_Atom(new_Atom_with_Symbol(new_symbol_nil()));
  cons->cdr = new_Object_with_Atom(new_Atom_with_Symbol(new_symbol_nil()));
  return cons;
}

void delete_Cons(Cons * cons)
{
  delete_Object(cons->car);
  delete_Object(cons->cdr);
}

int parse_Cons(const char * str, Cons ** cons)
{
  const char * p = str;
  while(!is_begin_list_char(*p) && !is_end_string_char(*p)) ++p;

  if(is_end_string_char(*p))
  {
    *cons = NULL;
    return p - str;
  }

  *cons = new_Cons();

  ++p;
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

  while(is_white_space_char(*p) && !is_end_string_char(*p)) ++p;

  // cdr part
  if(is_cons_dot_char(*p))
  {
    ++p;
    Atom * atom;
    p += parse_Atom(p, &atom);
    (*cons)->cdr = new_Object_with_Atom(atom);
  }
  else
  {
    Cons * c;
    p += parse_Cons(p, &c);
  }

  while(!is_end_list_char(*p) && !is_end_string_char(*p)) ++p;
  ++p;

  return p - str;
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
    ASSERT_STRING_EQUAL("a", get_symbol_name((Atom*)cons->car->ptr));
    ASSERT_STRING_EQUAL("b", get_symbol_name((Atom*)cons->cdr->ptr));
    delete_Cons(cons);
  }

  {
    Cons * cons;
    const char * s = " (a  .  b) ";
    ASSERT_INT_EQAUL(10, parse_Cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name((Atom*)cons->car->ptr));
    ASSERT_STRING_EQUAL("b", get_symbol_name((Atom*)cons->cdr->ptr));
    delete_Cons(cons);
  }

  {
    Cons * cons;
    const char * s = " (a1b2#!a  .  b*0f1@:) ";
    ASSERT_INT_EQAUL(22, parse_Cons(s, &cons));
    ASSERT_STRING_EQUAL("a1b2#!a", get_symbol_name((Atom*)cons->car->ptr));
    ASSERT_STRING_EQUAL("b*0f1@:", get_symbol_name((Atom*)cons->cdr->ptr));
    delete_Cons(cons);
  }

}
