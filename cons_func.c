/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include "cons_func.h"
#include "object_func.h"
#include "symbol.h"
#include "unittest.h"
#include "stringutils.h"

lisp_object * allocate_Cons()
{
  lisp_object * cons = (lisp_object*)malloc(sizeof(lisp_object));
  cons->type = LISP_OBJECT_CONS;
  cons->sub_type = LISP_OBJECT_ATOM_NONE;
  cons->atom = NULL;
  return cons;
}

lisp_object * new_Cons(lisp_object * car, lisp_object * cdr)
{
  lisp_object * cons = allocate_Cons();
  cons->car = car;
  cons->cdr = cdr;
  return cons;
}

int parse_Cons_internal(const char * str, lisp_object ** cons)
{
  const char * p = str;

  p += skip_chars_while(is_white_space_char, p);

  if(is_end_string_char(*p))
  {
    *cons = NULL;
    return p - str;
  }

  *cons = new_Cons(new_Atom_with_symbol(symbol_nil()),
                   new_Atom_with_symbol(symbol_nil()));

  boolean begin_with_begin_list_char = FALSE;
  if(is_begin_list_char(*p))
  {
    begin_with_begin_list_char = TRUE;
  }

  // car part
  /* printf("car = %d %s\n", begin_with_begin_list_char, p); */
  if(begin_with_begin_list_char)
  {
    ++p;
    lisp_object * c;
    p += parse_Cons_internal(p, &c);
    (*cons)->car = c;
  }
  else
  {
    lisp_object * atom;
    p += parse_Atom(p, &atom);
    (*cons)->car = atom;
  }

  p += skip_chars_while(is_white_space_char, p);

  // cdr part
  /* printf("cdr = %s\n", p); */
  if(is_cons_dot_char(*p))
  {
    ++p;
    lisp_object * atom;
    p += parse_Atom(p, &atom);
    (*cons)->cdr = atom;
  }
  else if(is_end_string_char(*p) ||
          is_end_list_char(*p))
  {
    (*cons)->cdr = new_Atom_with_symbol(symbol_nil());
  }
  else
  {
    lisp_object * tmp;
    p += parse_Cons_internal(p, &tmp);
    (*cons)->cdr = tmp;
  }

  if(begin_with_begin_list_char == TRUE)
  {
    p += skip_chars_while_not(is_end_list_char, p);
    if(is_end_list_char(*p)) ++p;
  }

  return p - str;
}

int parse_Cons(const char * str, lisp_object ** cons)
{
  const char * p = str;
  p += skip_chars_while_not(is_begin_list_char, p);
  if(is_begin_list_char(*p)) ++p;

  p += parse_Cons_internal(p, cons);

  p += skip_chars_while_not(is_end_list_char, p);
  if(is_end_list_char(*p)) ++p;

  return p - str;
}

lisp_object * car_as_lisp_object(const lisp_object * cons)
{
  return cons->car;
}

lisp_object * cdr_as_lisp_object(const lisp_object * cons)
{
  return cons->cdr;
}

lisp_object * nth_as_lisp_object(const lisp_object * cons, int n)
{
  n = (n < 0) ? 0 : n;

  const lisp_object * tmp = cons;
  while(n > 0)
  {
    tmp = cdr_as_lisp_object(tmp);
    --n;
  }

  return tmp->car;
}

lisp_object * nth_cdr(const lisp_object * cons, int n)
{
  n = (n < 0) ? 0 : n;

  const lisp_object * tmp = cons;
  while(n > 0)
  {
    tmp = cdr_as_lisp_object(tmp);
    --n;
  }

  return cdr_as_lisp_object(tmp);
}

const char * print_Atom(const lisp_object * atom)
{
  return allocate_string(get_symbol_name(atom));
}

void print_Cons_internal(const lisp_object * cons)
{
  if(cons == NULL)
  {
    printf("nil\n");
    return;
  }

  if(car_as_lisp_object(cons)->type == LISP_OBJECT_CONS)
  {
    printf("(");
    print_Cons_internal(car_as_lisp_object(cons));
  }
  else if(car_as_lisp_object(cons)->type == LISP_OBJECT_ATOM)
  {
    const char * s = print_Atom(car_as_lisp_object(cons));
    printf("%s", s);
    free((void*)s);
  }

  printf(" ");

  if(cdr_as_lisp_object(cons)->type == LISP_OBJECT_CONS)
  {
    print_Cons_internal(cdr_as_lisp_object(cons));
  }
  else if(cdr_as_lisp_object(cons)->type == LISP_OBJECT_ATOM)
  {
    printf(". ");
    const char * s = print_Atom(cdr_as_lisp_object(cons));
    printf("%s", s);
    free((void*)s);
    printf(")");
  }
}

void print_Cons(const lisp_object * cons)
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
    lisp_object * cons;
    const char * s = " ";
    ASSERT_INT_EQAUL(1, parse_Cons_internal(s, &cons));
    /* print_Cons(cons); */
    ASSERT_NULL(cons);
  }

  {
    lisp_object * cons;
    const char * s = " a ";
    ASSERT_INT_EQAUL(3, parse_Cons_internal(s, &cons));
    /* print_Cons(cons); */
    ASSERT_STRING_EQUAL("a", get_symbol_name(car_as_lisp_object(cons)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(cons, 0)));
    lisp_object_delete(cons);
  }

  {
    lisp_object * cons;
    const char * s = " a b ";
    ASSERT_INT_EQAUL(5, parse_Cons_internal(s, &cons));
    /* print_Cons(cons); */
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_lisp_object(cons, 0)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_lisp_object(cons, 1)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(cons, 1)));
    lisp_object_delete(cons);
  }

  {
    lisp_object * cons;
    const char * s = " (a) ";
    ASSERT_INT_EQAUL(4, parse_Cons_internal(s, &cons));
    /* print_Cons(cons); */
    lisp_object * car = car_as_lisp_object(cons);
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_lisp_object(car, 0)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(cdr_as_lisp_object(car)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(cdr_as_lisp_object(cons)));
    lisp_object_delete(cons);
  }

  {
    lisp_object * cons;
    const char * s = " ((a)) ";
    ASSERT_INT_EQAUL(6, parse_Cons_internal(s, &cons));
    /* print_Cons(cons); */
    lisp_object * car = car_as_lisp_object(cons);
    lisp_object * caar = car_as_lisp_object(car);
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_lisp_object(caar, 0)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(cdr_as_lisp_object(caar)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(cdr_as_lisp_object(cons)));
    lisp_object_delete(cons);
  }

  {
    lisp_object * cons;
    const char * s = " ((a b)) ";
    ASSERT_INT_EQAUL(8, parse_Cons_internal(s, &cons));
    /* print_Cons(cons); */
    lisp_object * car = car_as_lisp_object(cons);
    lisp_object * caar = car_as_lisp_object(car);
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_lisp_object(caar, 0)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_lisp_object(caar, 1)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(car->cdr));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(cdr_as_lisp_object(cons)));
    lisp_object_delete(cons);
  }
}

TEST_CASE(test_parse_Cons)
{
  {
    lisp_object * cons;
    const char * s = " ";
    int size = parse_Cons(s, &cons);
    ASSERT_INT_EQAUL(1, size);
    ASSERT_NULL(cons);
  }

  {
    lisp_object * cons;
    const char * s = " (a) ";
    ASSERT_INT_EQAUL(4, parse_Cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(car_as_lisp_object(cons)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(cons, 0)));
    lisp_object_delete(cons);
  }

  {
    lisp_object * cons;
    const char * s = "(a . b)";
    ASSERT_INT_EQAUL(7, parse_Cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(car_as_lisp_object(cons)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_cdr(cons, 0)));
    lisp_object_delete(cons);
  }

  {
    lisp_object * cons;
    const char * s = " (a  .  b) ";
    ASSERT_INT_EQAUL(10, parse_Cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(car_as_lisp_object(cons)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_cdr(cons, 0)));
    lisp_object_delete(cons);
  }

  {
    lisp_object * cons;
    const char * s = " (a1b2#!a  .  b*0f1@:) ";
    ASSERT_INT_EQAUL(22, parse_Cons(s, &cons));
    ASSERT_STRING_EQUAL("a1b2#!a", get_symbol_name(car_as_lisp_object(cons)));
    ASSERT_STRING_EQUAL("b*0f1@:", get_symbol_name(cdr_as_lisp_object(cons)));
    lisp_object_delete(cons);
  }

  {
    lisp_object * cons;
    const char * s = " (a b) ";
    parse_Cons(s, &cons);
    ASSERT_INT_EQAUL(6, parse_Cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_lisp_object(cons, 0)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_lisp_object(cons, 1)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(cons, 1)));
    lisp_object_delete(cons);
  }

  {
    lisp_object * cons;
    const char * s = " (a b c) ";
    ASSERT_INT_EQAUL(8, parse_Cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_lisp_object(cons, 0)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_lisp_object(cons, 1)));
    ASSERT_STRING_EQUAL("c", get_symbol_name(nth_as_lisp_object(cons, 2)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(cons, 2)));
    lisp_object_delete(cons);
  }

  {
    lisp_object * cons;
    const char * s = " (a b c . d) ";
    ASSERT_INT_EQAUL(12, parse_Cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_lisp_object(cons, 0)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_lisp_object(cons, 1)));
    ASSERT_STRING_EQUAL("c", get_symbol_name(nth_as_lisp_object(cons, 2)));
    ASSERT_STRING_EQUAL("d", get_symbol_name(nth_cdr(cons, 2)));
    lisp_object_delete(cons);
  }

  {
    lisp_object * cons;
    const char * s = " ((a)) ";
    ASSERT_INT_EQAUL(6, parse_Cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(car_as_lisp_object(car_as_lisp_object(cons))));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(cons, 0)));
    lisp_object_delete(cons);
  }

  {
    lisp_object * cons;
    const char * s = " ((a b)) ";
    ASSERT_INT_EQAUL(8, parse_Cons(s, &cons));
    lisp_object * car = car_as_lisp_object(cons);
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_lisp_object(car, 0)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_lisp_object(car, 1)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(car, 1)));
    lisp_object_delete(cons);
  }

  {
    lisp_object * cons;
    const char * s = " (a (b c)) ";
    ASSERT_INT_EQAUL(10, parse_Cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_lisp_object(cons, 0)));
    lisp_object * cdr = cdr_as_lisp_object(cons);
    lisp_object * cadr = car_as_lisp_object(cdr);
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_lisp_object(cadr, 0)));
    ASSERT_STRING_EQUAL("c", get_symbol_name(nth_as_lisp_object(cadr, 1)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(cadr, 1)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(cdr, 0)));
    lisp_object_delete(cons);
  }
}
