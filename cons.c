/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include "cons.h"
#include "symbol.h"
#include "atom.h"
#include "unittest.h"

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
  cons->car = (void*)new_symbol_nil();
  cons->cdr = (void*)new_symbol_nil();
  return cons;
}

void delete_Cons(Cons * cons)
{
  delete_Object(cons->car);
  delete_Object(cons->cdr);
}

int parse_Cons(const char * str, Cons ** cons)
{
  return 0;
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
