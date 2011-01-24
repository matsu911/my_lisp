/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include "cons_func.h"
#include "object_func.h"
#include "symbol.h"
#include "unittest.h"
#include "stringutils.h"

lisp_object * lisp_object_allocate_cons()
{
  lisp_object * cons = (lisp_object*)malloc(sizeof(lisp_object));

  cons->type     = LISP_OBJECT_CONS;
  cons->sub_type = LISP_OBJECT_ATOM_NONE;
  cons->atom     = NULL;

  return cons;
}

lisp_object * lisp_object_cons(lisp_object * car, lisp_object * cdr)
{
  lisp_object * cons = lisp_object_allocate_cons();
  cons->car = car;
  cons->cdr = cdr;
  return cons;
}

int lisp_object_parse_cons_internal(const char * str, lisp_object ** cons)
{
  const char * p = str;

  p += skip_chars_while(is_white_space_char, p);

  if(is_end_string_char(*p))
  {
    *cons = NULL;
    return p - str;
  }

  *cons = lisp_object_cons(lisp_object_atom_with_lisp_symbol(lisp_symbol_nil()),
                           lisp_object_atom_with_lisp_symbol(lisp_symbol_nil()));

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
    p += lisp_object_parse_cons_internal(p, &c);
    (*cons)->car = c;
  }
  else
  {
    lisp_object * atom;
    p += lisp_object_parse_atom(p, &atom);
    (*cons)->car = atom;
  }

  p += skip_chars_while(is_white_space_char, p);

  // cdr part
  /* printf("cdr = %s\n", p); */
  if(is_cons_dot_char(*p))
  {
    ++p;
    lisp_object * atom;
    p += lisp_object_parse_atom(p, &atom);
    (*cons)->cdr = atom;
  }
  else if(is_end_string_char(*p) ||
          is_end_list_char(*p))
  {
    (*cons)->cdr = lisp_object_atom_with_lisp_symbol(lisp_symbol_nil());
  }
  else
  {
    lisp_object * tmp;
    p += lisp_object_parse_cons_internal(p, &tmp);
    (*cons)->cdr = tmp;
  }

  if(begin_with_begin_list_char == TRUE)
  {
    p += skip_chars_while_not(is_end_list_char, p);
    if(is_end_list_char(*p)) ++p;
  }

  return p - str;
}

int lisp_object_parse_cons(const char * str, lisp_object ** cons)
{
  const char * p = str;
  p += skip_chars_while_not(is_begin_list_char, p);
  if(is_begin_list_char(*p)) ++p;

  p += lisp_object_parse_cons_internal(p, cons);

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

const char * lisp_object_print_atom(const lisp_object * atom)
{
  if(atom == NULL) return NULL;
  if(atom->sub_type == LISP_OBJECT_ATOM_SYMBOL)
    return allocate_string(get_symbol_name(atom));
  else if(atom->sub_type == LISP_OBJECT_ATOM_STRING)
    return allocate_string((char*)atom->atom);
  else if(atom->sub_type == LISP_OBJECT_ATOM_INTEGER)
  {
    char buff[256];
    sprintf(buff, "%d", *(int*)atom->atom);
    return allocate_string(buff);
  }
  else
    return NULL;
}

static void lisp_object_print_cons_internal(const lisp_object * cons)
{
  if(cons == NULL)
  {
    printf("nil\n");
    return;
  }

  if(car_as_lisp_object(cons)->type == LISP_OBJECT_CONS)
  {
    printf("(");
    lisp_object_print_cons_internal(car_as_lisp_object(cons));
  }
  else if(car_as_lisp_object(cons)->type == LISP_OBJECT_ATOM)
  {
    const char * s = lisp_object_print_atom(car_as_lisp_object(cons));
    printf("%s", s);
    free((void*)s);
  }

  printf(" ");

  if(cdr_as_lisp_object(cons)->type == LISP_OBJECT_CONS)
  {
    lisp_object_print_cons_internal(cdr_as_lisp_object(cons));
  }
  else if(cdr_as_lisp_object(cons)->type == LISP_OBJECT_ATOM)
  {
    printf(". ");
    const char * s = lisp_object_print_atom(cdr_as_lisp_object(cons));
    printf("%s", s);
    free((void*)s);
    printf(")");
  }
}

void lisp_object_print_cons(const lisp_object * cons)
{
  printf("\n");
  printf("(");
  lisp_object_print_cons_internal(cons);
  /* printf(")"); */
  printf("\n");
}

TEST_CASE(test_lisp_object_parse_cons_internal)
{
  {
    lisp_object * cons;
    const char * s = " ";
    ASSERT_INT_EQUAL(1, lisp_object_parse_cons_internal(s, &cons));
    /* lisp_object_print_cons(cons); */
    ASSERT_NULL(cons);
  }

  {
    lisp_object * cons;
    const char * s = " a ";
    ASSERT_INT_EQUAL(3, lisp_object_parse_cons_internal(s, &cons));
    /* lisp_object_print_cons(cons); */
    ASSERT_STRING_EQUAL("a", get_symbol_name(car_as_lisp_object(cons)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(cons, 0)));
    lisp_object_free(cons);
  }

  {
    lisp_object * cons;
    const char * s = " a b ";
    ASSERT_INT_EQUAL(5, lisp_object_parse_cons_internal(s, &cons));
    /* lisp_object_print_cons(cons); */
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_lisp_object(cons, 0)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_lisp_object(cons, 1)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(cons, 1)));
    lisp_object_free(cons);
  }

  {
    lisp_object * cons;
    const char * s = " (a) ";
    ASSERT_INT_EQUAL(4, lisp_object_parse_cons_internal(s, &cons));
    /* lisp_object_print_cons(cons); */
    lisp_object * car = car_as_lisp_object(cons);
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_lisp_object(car, 0)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(cdr_as_lisp_object(car)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(cdr_as_lisp_object(cons)));
    lisp_object_free(cons);
  }

  {
    lisp_object * cons;
    const char * s = " ((a)) ";
    ASSERT_INT_EQUAL(6, lisp_object_parse_cons_internal(s, &cons));
    /* lisp_object_print_cons(cons); */
    lisp_object * car = car_as_lisp_object(cons);
    lisp_object * caar = car_as_lisp_object(car);
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_lisp_object(caar, 0)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(cdr_as_lisp_object(caar)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(cdr_as_lisp_object(cons)));
    lisp_object_free(cons);
  }

  {
    lisp_object * cons;
    const char * s = " ((a b)) ";
    ASSERT_INT_EQUAL(8, lisp_object_parse_cons_internal(s, &cons));
    /* lisp_object_print_cons(cons); */
    lisp_object * car = car_as_lisp_object(cons);
    lisp_object * caar = car_as_lisp_object(car);
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_lisp_object(caar, 0)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_lisp_object(caar, 1)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(car->cdr));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(cdr_as_lisp_object(cons)));
    lisp_object_free(cons);
  }
}

TEST_CASE(test_lisp_object_parse_cons)
{
  {
    lisp_object * cons;
    const char * s = " ";
    int size = lisp_object_parse_cons(s, &cons);
    ASSERT_INT_EQUAL(1, size);
    ASSERT_NULL(cons);
  }

  {
    lisp_object * cons;
    const char * s = " (a) ";
    ASSERT_INT_EQUAL(4, lisp_object_parse_cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(car_as_lisp_object(cons)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(cons, 0)));
    lisp_object_free(cons);
  }

  {
    lisp_object * cons;
    const char * s = "(a . b)";
    ASSERT_INT_EQUAL(7, lisp_object_parse_cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(car_as_lisp_object(cons)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_cdr(cons, 0)));
    lisp_object_free(cons);
  }

  {
    lisp_object * cons;
    const char * s = " (a  .  b) ";
    ASSERT_INT_EQUAL(10, lisp_object_parse_cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(car_as_lisp_object(cons)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_cdr(cons, 0)));
    lisp_object_free(cons);
  }

  {
    lisp_object * cons;
    const char * s = " (a1b2#!a  .  b*0f1@:) ";
    ASSERT_INT_EQUAL(22, lisp_object_parse_cons(s, &cons));
    ASSERT_STRING_EQUAL("a1b2#!a", get_symbol_name(car_as_lisp_object(cons)));
    ASSERT_STRING_EQUAL("b*0f1@:", get_symbol_name(cdr_as_lisp_object(cons)));
    lisp_object_free(cons);
  }

  {
    lisp_object * cons;
    const char * s = " (a b) ";
    lisp_object_parse_cons(s, &cons);
    ASSERT_INT_EQUAL(6, lisp_object_parse_cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_lisp_object(cons, 0)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_lisp_object(cons, 1)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(cons, 1)));
    lisp_object_free(cons);
  }

  {
    lisp_object * cons;
    const char * s = " (a b c) ";
    ASSERT_INT_EQUAL(8, lisp_object_parse_cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_lisp_object(cons, 0)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_lisp_object(cons, 1)));
    ASSERT_STRING_EQUAL("c", get_symbol_name(nth_as_lisp_object(cons, 2)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(cons, 2)));
    lisp_object_free(cons);
  }

  {
    lisp_object * cons;
    const char * s = " (a b c . d) ";
    ASSERT_INT_EQUAL(12, lisp_object_parse_cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_lisp_object(cons, 0)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_lisp_object(cons, 1)));
    ASSERT_STRING_EQUAL("c", get_symbol_name(nth_as_lisp_object(cons, 2)));
    ASSERT_STRING_EQUAL("d", get_symbol_name(nth_cdr(cons, 2)));
    lisp_object_free(cons);
  }

  {
    lisp_object * cons;
    const char * s = " ((a)) ";
    ASSERT_INT_EQUAL(6, lisp_object_parse_cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(car_as_lisp_object(car_as_lisp_object(cons))));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(cons, 0)));
    lisp_object_free(cons);
  }

  {
    lisp_object * cons;
    const char * s = " ((a b)) ";
    ASSERT_INT_EQUAL(8, lisp_object_parse_cons(s, &cons));
    lisp_object * car = car_as_lisp_object(cons);
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_lisp_object(car, 0)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_lisp_object(car, 1)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(car, 1)));
    lisp_object_free(cons);
  }

  {
    lisp_object * cons;
    const char * s = " (a (b c)) ";
    ASSERT_INT_EQUAL(10, lisp_object_parse_cons(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_lisp_object(cons, 0)));
    lisp_object * cdr = cdr_as_lisp_object(cons);
    lisp_object * cadr = car_as_lisp_object(cdr);
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_lisp_object(cadr, 0)));
    ASSERT_STRING_EQUAL("c", get_symbol_name(nth_as_lisp_object(cadr, 1)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(cadr, 1)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(cdr, 0)));
    lisp_object_free(cons);
  }
}
