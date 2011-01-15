/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include "cons.h"
#include "symbol.h"
#include "atom.h"
#include "unittest.h"

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
  cons->car = (void*)new_Object_with_Atom(new_Atom_with_Symbol(new_symbol_nil()));
  cons->cdr = (void*)new_Object_with_Atom(new_Atom_with_Symbol(new_symbol_nil()));
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
  while(!is_begin_list_char(*p)) ++p;

  cons = new_Cons();
  
  return p - str;
}


TEST_CASE(test_parse_Cons)
{
  /* { */
  /*   const char * s = "abc"; */
  /*   const char * tmp = (char*)malloc(sizeof(char) * strlen(s)); */
  /*   ASSERT_TRUE(strcmp("", tmp) == 0); */
  /*   if(tmp != NULL) */
  /*     free((void*)tmp); */
  /* } */
}
