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

  p += skip_chars_while(is_white_space_char, p);

  if(is_end_string_char(*p))
  {
    *cons = NULL;
    return p - str;
  }

  *cons = new_Cons();

  LISP_BOOL begin_with_begin_list_char = LISP_FALSE;
  if(is_begin_list_char(*p))
  {
    begin_with_begin_list_char = LISP_TRUE;
    /* ++p; */
  }

  // car part
  /* printf("car = %d %s\n", begin_with_begin_list_char, p); */
  if(begin_with_begin_list_char)
  {
    ++p;
    Cons * c;
    p += parse_Cons_internal(p, &c);
    (*cons)->car = new_Object_with_Cons(c);
  }
  else
  {
    Atom * atom;
    p += parse_Atom(p, &atom);
    (*cons)->car = new_Object_with_Atom(atom);
  }

  p += skip_chars_while(is_white_space_char, p);

  // cdr part
  /* printf("cdr = %s\n", p); */
  if(is_cons_dot_char(*p))
  {
    ++p;
    Atom * atom;
    p += parse_Atom(p, &atom);
    (*cons)->cdr = new_Object_with_Atom(atom);
  }
  else if(is_end_string_char(*p) ||
          is_end_list_char(*p))
  {
    (*cons)->cdr = new_Object_with_Atom(new_Atom_with_Symbol(new_symbol_nil()));
  }
  else
  {
    Cons * tmp;
    p += parse_Cons_internal(p, &tmp);
    (*cons)->cdr = new_Object_with_Cons(tmp);
  }

  if(begin_with_begin_list_char == LISP_TRUE)
  {
    p += skip_chars_while_not(is_end_list_char, p);
    if(is_end_list_char(*p)) ++p;
  }

  return p - str;
}

int parse_Cons(const char * str, Cons ** cons)
{
  const char * p = str;
  p += skip_chars_while_not(is_begin_list_char, p);
  if(is_begin_list_char(*p)) ++p;

  p += parse_Cons_internal(p, cons);

  p += skip_chars_while_not(is_end_list_char, p);
  if(is_end_list_char(*p)) ++p;

  return p - str;
}

Object * car_as_Object(const Cons * cons)
{
  return cons->car;
}

Object * cdr_as_Object(const Cons * cons)
{
  return cons->cdr;
}

Atom * car_as_Atom(const Cons * cons)
{
  return (Atom*)car_as_Object(cons)->ptr;
}

Atom * cdr_as_Atom(const Cons * cons)
{
  return (Atom*)cdr_as_Object(cons)->ptr;
}

Cons * car_as_Cons(const Cons * cons)
{
  return (Cons*)car_as_Object(cons)->ptr;
}

Cons * cdr_as_Cons(const Cons * cons)
{
  return (Cons*)cdr_as_Object(cons)->ptr;
}

Atom * nth_as_Atom(const Cons * cons, int n)
{
  n = (n < 0) ? 0 : n;

  const Cons * tmp = cons;
  while(n > 0)
  {
    tmp = cdr_as_Cons(tmp);
    --n;
  }

  return car_as_Atom(tmp);
}

Object * nth_cdr(const Cons * cons, int n)
{
  n = (n < 0) ? 0 : n;

  const Cons * tmp = cons;
  while(n > 0)
  {
    tmp = cdr_as_Cons(tmp);
    --n;
  }

  return cdr_as_Object(tmp);
}

const char * print_Atom(const Atom * atom)
{
  return allocate_string(get_symbol_name(atom));
}

void print_Cons_internal(const Cons * cons)
{
  if(cons == NULL)
  {
    printf("nil\n");
    return;
  }

  if(cons->car->type == OBJECT_CONS)
  {
    printf("(");
    print_Cons_internal((Cons*)cons->car->ptr);
  }
  else if(cons->car->type == OBJECT_ATOM)
  {
    const char * s = print_Atom((Atom*)cons->car->ptr);
    printf("%s", s);
    free((void*)s);
  }

  printf(" ");

  if(cons->cdr->type == OBJECT_CONS)
  {
    print_Cons_internal((Cons*)cons->cdr->ptr);
  }
  else if(cons->cdr->type == OBJECT_ATOM)
  {
    printf(". ");
    const char * s = print_Atom((Atom*)cons->cdr->ptr);
    printf("%s", s);
    free((void*)s);
    printf(")");
  }
}

void print_Cons(const Cons * cons)
{
  printf("\n");
  printf("(");
  print_Cons_internal(cons);
  /* printf(")"); */
  printf("\n");
}

TEST_CASE(test_parse_Cons_internal)
{
  {
    Cons * cons;
    const char * s = " ";
    ASSERT_INT_EQAUL(1, parse_Cons_internal(s, &cons));
    /* print_Cons(cons); */
    ASSERT_TRUE(cons == NULL);
  }

  {
    Cons * cons;
    const char * s = " a ";
    ASSERT_INT_EQAUL(3, parse_Cons_internal(s, &cons));
    /* print_Cons(cons); */
    ASSERT_STRING_EQUAL("a", get_symbol_name(car_as_Atom(cons)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name((Atom*)nth_cdr(cons, 0)->ptr));
    delete_Cons(cons);
  }

  {
    Cons * cons;
    const char * s = " a b ";
    ASSERT_INT_EQAUL(5, parse_Cons_internal(s, &cons));
    /* print_Cons(cons); */
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_Atom(cons, 0)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_Atom(cons, 1)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name((Atom*)nth_cdr(cons, 1)->ptr));
    delete_Cons(cons);
  }

  {
    Cons * cons;
    const char * s = " (a) ";
    ASSERT_INT_EQAUL(4, parse_Cons_internal(s, &cons));
    /* print_Cons(cons); */
    Cons * car = car_as_Cons(cons);
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_Atom(car, 0)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(cdr_as_Atom(car)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(cdr_as_Atom(cons)));
    delete_Cons(cons);
  }

  {
    Cons * cons;
    const char * s = " ((a)) ";
    ASSERT_INT_EQAUL(6, parse_Cons_internal(s, &cons));
    /* print_Cons(cons); */
    Cons * car = car_as_Cons(cons);
    Cons * caar = car_as_Cons(car);
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_Atom(caar, 0)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(cdr_as_Atom(caar)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(cdr_as_Atom(cons)));
    delete_Cons(cons);
  }

  {
    Cons * cons;
    const char * s = " ((a b)) ";
    ASSERT_INT_EQAUL(8, parse_Cons_internal(s, &cons));
    /* print_Cons(cons); */
    Cons * car = car_as_Cons(cons);
    Cons * caar = car_as_Cons(car);
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_Atom(caar, 0)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_Atom(caar, 1)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name((Atom*)((Cons*)nth_cdr(caar, 1))->cdr));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(cdr_as_Atom(cons)));
    delete_Cons(cons);
  }
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
    const char * s = " (a) ";
    ASSERT_INT_EQAUL(4, parse_Cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(car_as_Atom(cons)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name((Atom*)nth_cdr(cons, 0)->ptr));
    delete_Cons(cons);
  }

  {
    Cons * cons;
    const char * s = "(a . b)";
    ASSERT_INT_EQAUL(7, parse_Cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(car_as_Atom(cons)));
    ASSERT_STRING_EQUAL("b", get_symbol_name((Atom*)nth_cdr(cons, 0)->ptr));
    delete_Cons(cons);
  }

  {
    Cons * cons;
    const char * s = " (a  .  b) ";
    ASSERT_INT_EQAUL(10, parse_Cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(car_as_Atom(cons)));
    ASSERT_STRING_EQUAL("b", get_symbol_name((Atom*)nth_cdr(cons, 0)->ptr));
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
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_Atom(cons, 0)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_Atom(cons, 1)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name((Atom*)nth_cdr(cons, 1)->ptr));
    delete_Cons(cons);
  }

  {
    Cons * cons;
    const char * s = " (a b c) ";
    ASSERT_INT_EQAUL(8, parse_Cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_Atom(cons, 0)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_Atom(cons, 1)));
    ASSERT_STRING_EQUAL("c", get_symbol_name(nth_as_Atom(cons, 2)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name((Atom*)nth_cdr(cons, 2)->ptr));
    delete_Cons(cons);
  }

  {
    Cons * cons;
    const char * s = " (a b c . d) ";
    ASSERT_INT_EQAUL(12, parse_Cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_Atom(cons, 0)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_Atom(cons, 1)));
    ASSERT_STRING_EQUAL("c", get_symbol_name(nth_as_Atom(cons, 2)));
    ASSERT_STRING_EQUAL("d", get_symbol_name((Atom*)nth_cdr(cons, 2)->ptr));
    delete_Cons(cons);
  }

  {
    Cons * cons;
    const char * s = " ((a)) ";
    ASSERT_INT_EQAUL(6, parse_Cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(car_as_Atom(car_as_Cons(cons))));
    ASSERT_STRING_EQUAL("nil", get_symbol_name((Atom*)nth_cdr(cons, 0)->ptr));
    delete_Cons(cons);
  }

  {
    Cons * cons;
    const char * s = " ((a b)) ";
    ASSERT_INT_EQAUL(8, parse_Cons(s, &cons));
    Cons * car = car_as_Cons(cons);
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_Atom(car, 0)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_Atom(car, 1)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name((Atom*)nth_cdr(car, 1)->ptr));
    delete_Cons(cons);
  }

  {
    Cons * cons;
    const char * s = " (a (b c)) ";
    ASSERT_INT_EQAUL(10, parse_Cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_Atom(cons, 0)));
    Cons * cdr = cdr_as_Cons(cons);
    Cons * cadr = car_as_Cons(cdr);
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_Atom(cadr, 0)));
    ASSERT_STRING_EQUAL("c", get_symbol_name(nth_as_Atom(cadr, 1)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name((Atom*)nth_cdr(cadr, 1)->ptr));
    ASSERT_STRING_EQUAL("nil", get_symbol_name((Atom*)nth_cdr(cdr, 0)->ptr));
    delete_Cons(cons);
  }
}
