/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include "cons_func.h"
#include "object_func.h"
#include "symbol.h"
#include "unittest.h"
#include "stringutils.h"

Object * allocate_Cons()
{
  Object * cons = (Object*)malloc(sizeof(Object));
  cons->type = OBJECT_CONS;
  cons->sub_type = OBJECT_ATOM_NONE;
  cons->atom = NULL;
  return cons;
}

Object * new_Cons(Object * car, Object * cdr)
{
  Object * cons = allocate_Cons();
  cons->car = car;
  cons->cdr = cdr;
  return cons;
}

int parse_Cons_internal(const char * str, Object ** cons)
{
  const char * p = str;

  p += skip_chars_while(is_white_space_char, p);

  if(is_end_string_char(*p))
  {
    *cons = NULL;
    return p - str;
  }

  *cons = new_Cons(new_Atom_with_Symbol(symbol_nil()),
                   new_Atom_with_Symbol(symbol_nil()));

  LISP_BOOL begin_with_begin_list_char = LISP_FALSE;
  if(is_begin_list_char(*p))
  {
    begin_with_begin_list_char = LISP_TRUE;
  }

  // car part
  /* printf("car = %d %s\n", begin_with_begin_list_char, p); */
  if(begin_with_begin_list_char)
  {
    ++p;
    Object * c;
    p += parse_Cons_internal(p, &c);
    (*cons)->car = c;
  }
  else
  {
    Object * atom;
    p += parse_Atom(p, &atom);
    (*cons)->car = atom;
  }

  p += skip_chars_while(is_white_space_char, p);

  // cdr part
  /* printf("cdr = %s\n", p); */
  if(is_cons_dot_char(*p))
  {
    ++p;
    Object * atom;
    p += parse_Atom(p, &atom);
    (*cons)->cdr = atom;
  }
  else if(is_end_string_char(*p) ||
          is_end_list_char(*p))
  {
    (*cons)->cdr = new_Atom_with_Symbol(symbol_nil());
  }
  else
  {
    Object * tmp;
    p += parse_Cons_internal(p, &tmp);
    (*cons)->cdr = tmp;
  }

  if(begin_with_begin_list_char == LISP_TRUE)
  {
    p += skip_chars_while_not(is_end_list_char, p);
    if(is_end_list_char(*p)) ++p;
  }

  return p - str;
}

int parse_Cons(const char * str, Object ** cons)
{
  const char * p = str;
  p += skip_chars_while_not(is_begin_list_char, p);
  if(is_begin_list_char(*p)) ++p;

  p += parse_Cons_internal(p, cons);

  p += skip_chars_while_not(is_end_list_char, p);
  if(is_end_list_char(*p)) ++p;

  return p - str;
}

Object * car_as_Object(const Object * cons)
{
  return cons->car;
}

Object * cdr_as_Object(const Object * cons)
{
  return cons->cdr;
}

Object * nth_as_Object(const Object * cons, int n)
{
  n = (n < 0) ? 0 : n;

  const Object * tmp = cons;
  while(n > 0)
  {
    tmp = cdr_as_Object(tmp);
    --n;
  }

  return tmp->car;
}

Object * nth_cdr(const Object * cons, int n)
{
  n = (n < 0) ? 0 : n;

  const Object * tmp = cons;
  while(n > 0)
  {
    tmp = cdr_as_Object(tmp);
    --n;
  }

  return cdr_as_Object(tmp);
}

const char * print_Atom(const Object * atom)
{
  return allocate_string(get_symbol_name(atom));
}

void print_Cons_internal(const Object * cons)
{
  if(cons == NULL)
  {
    printf("nil\n");
    return;
  }

  if(car_as_Object(cons)->type == OBJECT_CONS)
  {
    printf("(");
    print_Cons_internal(car_as_Object(cons));
  }
  else if(car_as_Object(cons)->type == OBJECT_ATOM)
  {
    const char * s = print_Atom(car_as_Object(cons));
    printf("%s", s);
    free((void*)s);
  }

  printf(" ");

  if(cdr_as_Object(cons)->type == OBJECT_CONS)
  {
    print_Cons_internal(cdr_as_Object(cons));
  }
  else if(cdr_as_Object(cons)->type == OBJECT_ATOM)
  {
    printf(". ");
    const char * s = print_Atom(cdr_as_Object(cons));
    printf("%s", s);
    free((void*)s);
    printf(")");
  }
}

void print_Cons(const Object * cons)
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
    Object * cons;
    const char * s = " ";
    ASSERT_INT_EQAUL(1, parse_Cons_internal(s, &cons));
    /* print_Cons(cons); */
    ASSERT_NULL(cons);
  }

  {
    Object * cons;
    const char * s = " a ";
    ASSERT_INT_EQAUL(3, parse_Cons_internal(s, &cons));
    /* print_Cons(cons); */
    ASSERT_STRING_EQUAL("a", get_symbol_name(car_as_Object(cons)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(cons, 0)));
    delete_Object(cons);
  }

  {
    Object * cons;
    const char * s = " a b ";
    ASSERT_INT_EQAUL(5, parse_Cons_internal(s, &cons));
    /* print_Cons(cons); */
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_Object(cons, 0)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_Object(cons, 1)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(cons, 1)));
    delete_Object(cons);
  }

  {
    Object * cons;
    const char * s = " (a) ";
    ASSERT_INT_EQAUL(4, parse_Cons_internal(s, &cons));
    /* print_Cons(cons); */
    Object * car = car_as_Object(cons);
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_Object(car, 0)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(cdr_as_Object(car)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(cdr_as_Object(cons)));
    delete_Object(cons);
  }

  {
    Object * cons;
    const char * s = " ((a)) ";
    ASSERT_INT_EQAUL(6, parse_Cons_internal(s, &cons));
    /* print_Cons(cons); */
    Object * car = car_as_Object(cons);
    Object * caar = car_as_Object(car);
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_Object(caar, 0)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(cdr_as_Object(caar)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(cdr_as_Object(cons)));
    delete_Object(cons);
  }

  {
    Object * cons;
    const char * s = " ((a b)) ";
    ASSERT_INT_EQAUL(8, parse_Cons_internal(s, &cons));
    /* print_Cons(cons); */
    Object * car = car_as_Object(cons);
    Object * caar = car_as_Object(car);
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_Object(caar, 0)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_Object(caar, 1)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(car->cdr));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(cdr_as_Object(cons)));
    delete_Object(cons);
  }
}

TEST_CASE(test_parse_Cons)
{
  {
    Object * cons;
    const char * s = " ";
    int size = parse_Cons(s, &cons);
    ASSERT_INT_EQAUL(1, size);
    ASSERT_NULL(cons);
  }

  {
    Object * cons;
    const char * s = " (a) ";
    ASSERT_INT_EQAUL(4, parse_Cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(car_as_Object(cons)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(cons, 0)));
    delete_Object(cons);
  }

  {
    Object * cons;
    const char * s = "(a . b)";
    ASSERT_INT_EQAUL(7, parse_Cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(car_as_Object(cons)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_cdr(cons, 0)));
    delete_Object(cons);
  }

  {
    Object * cons;
    const char * s = " (a  .  b) ";
    ASSERT_INT_EQAUL(10, parse_Cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(car_as_Object(cons)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_cdr(cons, 0)));
    delete_Object(cons);
  }

  {
    Object * cons;
    const char * s = " (a1b2#!a  .  b*0f1@:) ";
    ASSERT_INT_EQAUL(22, parse_Cons(s, &cons));
    ASSERT_STRING_EQUAL("a1b2#!a", get_symbol_name(car_as_Object(cons)));
    ASSERT_STRING_EQUAL("b*0f1@:", get_symbol_name(cdr_as_Object(cons)));
    delete_Object(cons);
  }

  {
    Object * cons;
    const char * s = " (a b) ";
    parse_Cons(s, &cons);
    ASSERT_INT_EQAUL(6, parse_Cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_Object(cons, 0)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_Object(cons, 1)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(cons, 1)));
    delete_Object(cons);
  }

  {
    Object * cons;
    const char * s = " (a b c) ";
    ASSERT_INT_EQAUL(8, parse_Cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_Object(cons, 0)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_Object(cons, 1)));
    ASSERT_STRING_EQUAL("c", get_symbol_name(nth_as_Object(cons, 2)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(cons, 2)));
    delete_Object(cons);
  }

  {
    Object * cons;
    const char * s = " (a b c . d) ";
    ASSERT_INT_EQAUL(12, parse_Cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_Object(cons, 0)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_Object(cons, 1)));
    ASSERT_STRING_EQUAL("c", get_symbol_name(nth_as_Object(cons, 2)));
    ASSERT_STRING_EQUAL("d", get_symbol_name(nth_cdr(cons, 2)));
    delete_Object(cons);
  }

  {
    Object * cons;
    const char * s = " ((a)) ";
    ASSERT_INT_EQAUL(6, parse_Cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(car_as_Object(car_as_Object(cons))));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(cons, 0)));
    delete_Object(cons);
  }

  {
    Object * cons;
    const char * s = " ((a b)) ";
    ASSERT_INT_EQAUL(8, parse_Cons(s, &cons));
    Object * car = car_as_Object(cons);
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_Object(car, 0)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_Object(car, 1)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(car, 1)));
    delete_Object(cons);
  }

  {
    Object * cons;
    const char * s = " (a (b c)) ";
    ASSERT_INT_EQAUL(10, parse_Cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_Object(cons, 0)));
    Object * cdr = cdr_as_Object(cons);
    Object * cadr = car_as_Object(cdr);
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_Object(cadr, 0)));
    ASSERT_STRING_EQUAL("c", get_symbol_name(nth_as_Object(cadr, 1)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(cadr, 1)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(cdr, 0)));
    delete_Object(cons);
  }
}
