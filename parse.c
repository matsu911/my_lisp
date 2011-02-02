/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include <gc.h>
#include "parse.h"
#include "symbol.h"
#include "atom.h"
#include "cons_func.h"
#include "unittest.h"
#include "stringutils.h"
#include "object_func.h"

static int lisp_object_parse_atom(const char * str, lisp_object ** atom)
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

  char * name = (char*)GC_MALLOC_ATOMIC(sizeof(char) * (end - beg + 1));
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

  *atom = lisp_object_atom();
  if(strlen(name) > 0)
  {
    if(strcmp(name, "nil") == 0)
    {
      (*atom)->atom     = lisp_symbol_nil();
      (*atom)->type     = LISP_OBJECT_ATOM;
      (*atom)->sub_type = LISP_OBJECT_ATOM_SYMBOL;
    }
    else if(is_number_string(name))
    {
      int * p = (int*)GC_MALLOC_ATOMIC(sizeof(int));
      *p      = atoi(name);

      (*atom)->atom     = p;
      (*atom)->type     = LISP_OBJECT_ATOM;
      (*atom)->sub_type = LISP_OBJECT_ATOM_INTEGER;
    }
    else
    {
      lisp_symbol * sym = lisp_symbol_allocate();
      sym->name         = name;
      (*atom)->atom     = sym;
      (*atom)->type     = LISP_OBJECT_ATOM;
      (*atom)->sub_type = LISP_OBJECT_ATOM_SYMBOL;
    }
  }
  else
  {
    (*atom)->atom     = lisp_symbol_nil();
    (*atom)->type     = LISP_OBJECT_ATOM;
    (*atom)->sub_type = LISP_OBJECT_ATOM_SYMBOL;
  }

  return end - str;
}

static int lisp_object_parse_cons(const char * str, lisp_object ** cons);
static int lisp_object_parse_cons_internal(const char * str, lisp_object ** cons);

static int lisp_object_parse_cons_internal2(const char * str, lisp_object * target)
{
  char * p = str;

  char c;
  while((c = *(p + skip_chars_while(is_white_space_char, p))) &&
        !is_end_list_char(c) && !is_end_string_char(c))
  {
    lisp_object * tmp;
    p += lisp_object_parse_cons_internal(p, &tmp);
    target->cdr = tmp;
    target = tmp;
  }

  return p - str;
}

static int lisp_object_parse_cons_internal(const char * str, lisp_object ** cons)
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
    lisp_object * obj;
    p += lisp_object_parse_cons_internal(p, &obj);
    p += lisp_object_parse_cons_internal2(p, obj);
    (*cons)->car = obj;
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
    p += lisp_object_parse_cons_internal2(p, *cons);
  }

  if(begin_with_begin_list_char == TRUE)
  {
    p += skip_chars_while_not(is_end_list_char, p);
    if(is_end_list_char(*p)) ++p;
  }

  return p - str;
}

static int lisp_object_parse_cons(const char * str, lisp_object ** cons)
{
  const char * p = str;
  p += skip_chars_while_not(is_begin_list_char, p);
  if(is_begin_list_char(*p))     ++p;

  p += lisp_object_parse_cons_internal(p, cons);
  p += skip_chars_while_not(is_end_list_char, p);
  if(is_end_list_char(*p)) ++p;

  return p - str;
}

int lisp_object_parse(const char * str, lisp_object ** cons)
{
  if(is_begin_list_char(*(str + skip_chars_while_not(is_begin_list_char, str))))
    return lisp_object_parse_cons(str, cons);
  else
    return lisp_object_parse_atom(str, cons);
}

static lisp_object * nth_as_lisp_object(const lisp_object * cons, int n)
{
  n = (n < 0) ? 0 : n;

  const lisp_object * tmp = cons;
  while(n > 0)
  {
    tmp = CDR(tmp);
    --n;
  }

  return tmp->car;
}

static lisp_object * nth_cdr(const lisp_object * cons, int n)
{
  n = (n < 0) ? 0 : n;

  const lisp_object * tmp = cons;
  while(n > 0)
  {
    tmp = CDR(tmp);
    --n;
  }

  return CDR(tmp);
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
    const char * s = "nil";
    int size = lisp_object_parse_cons_internal(s, &cons);
    ASSERT_INT_EQUAL(3, size);
    ASSERT_NOT_NULL(cons);
    ASSERT_TRUE(is_lisp_symbol_nil(CAR(cons)->atom));
  }

  {
    lisp_object * cons;
    const char * s = " a ";
    ASSERT_INT_EQUAL(3, lisp_object_parse_cons_internal(s, &cons));
    /* lisp_object_print_cons(cons); */
    ASSERT_STRING_EQUAL("a", get_symbol_name(CAR(cons)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(cons, 0)));
    /* lisp_object_free(cons); */
  }

  {
    lisp_object * cons;
    const char * s = " a b ";
    ASSERT_INT_EQUAL(5, lisp_object_parse_cons_internal(s, &cons));
    /* lisp_object_print_cons(cons); */
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_lisp_object(cons, 0)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_lisp_object(cons, 1)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(cons, 1)));
    /* lisp_object_free(cons); */
  }

  {
    lisp_object * cons;
    const char * s = " (a) ";
    ASSERT_INT_EQUAL(4, lisp_object_parse_cons_internal(s, &cons));
    /* lisp_object_print_cons(cons); */
    lisp_object * car = CAR(cons);
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_lisp_object(car, 0)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(CDR(car)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(CDR(cons)));
    /* lisp_object_free(cons); */
  }

  {
    lisp_object * cons;
    const char * s = " ((a)) ";
    ASSERT_INT_EQUAL(6, lisp_object_parse_cons_internal(s, &cons));
    /* lisp_object_print_cons(cons); */
    lisp_object * car = CAR(cons);
    lisp_object * caar = CAR(car);
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_lisp_object(caar, 0)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(CDR(caar)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(CDR(cons)));
    /* lisp_object_free(cons); */
  }

  {
    lisp_object * cons;
    const char * s = " ((a b)) ";
    ASSERT_INT_EQUAL(8, lisp_object_parse_cons_internal(s, &cons));
    /* lisp_object_print_cons(cons); */
    lisp_object * car = CAR(cons);
    lisp_object * caar = CAR(car);
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_lisp_object(caar, 0)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_lisp_object(caar, 1)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(car->cdr));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(CDR(cons)));
    /* lisp_object_free(cons); */
  }
}

TEST_CASE(test_lisp_object_parse)
{
  {
    lisp_object * cons;
    const char * s = " ";
    int size = lisp_object_parse(s, &cons);
    ASSERT_INT_EQUAL(1, size);
    ASSERT_NOT_NULL(cons);
    ASSERT_TRUE(is_lisp_symbol_nil(cons->atom));
  }

  {
    lisp_object * cons;
    const char * s = "nil";
    int size = lisp_object_parse(s, &cons);
    ASSERT_INT_EQUAL(3, size);
    ASSERT_NOT_NULL(cons);
    ASSERT_TRUE(is_lisp_symbol_nil(cons->atom));
  }

  {
    lisp_object * cons;
    const char * s = " (a) ";
    ASSERT_INT_EQUAL(4, lisp_object_parse(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(CAR(cons)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(cons, 0)));
    /* lisp_object_free(cons); */
  }

  {
    lisp_object * cons;
    const char * s = "(a . b)";
    ASSERT_INT_EQUAL(7, lisp_object_parse(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(CAR(cons)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_cdr(cons, 0)));
    /* lisp_object_free(cons); */
  }

  {
    lisp_object * cons;
    const char * s = " (a  .  b) ";
    ASSERT_INT_EQUAL(10, lisp_object_parse(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(CAR(cons)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_cdr(cons, 0)));
    /* lisp_object_free(cons); */
  }

  {
    lisp_object * cons;
    const char * s = " (a1b2#!a  .  b*0f1@:) ";
    ASSERT_INT_EQUAL(22, lisp_object_parse(s, &cons));
    ASSERT_STRING_EQUAL("a1b2#!a", get_symbol_name(CAR(cons)));
    ASSERT_STRING_EQUAL("b*0f1@:", get_symbol_name(CDR(cons)));
    /* lisp_object_free(cons); */
  }

  {
    lisp_object * cons;
    const char * s = " (a b) ";
    lisp_object_parse(s, &cons);
    ASSERT_INT_EQUAL(6, lisp_object_parse(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_lisp_object(cons, 0)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_lisp_object(cons, 1)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(cons, 1)));
    /* lisp_object_free(cons); */
  }

  {
    lisp_object * cons;
    const char * s = " (a b c) ";
    ASSERT_INT_EQUAL(8, lisp_object_parse(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_lisp_object(cons, 0)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_lisp_object(cons, 1)));
    ASSERT_STRING_EQUAL("c", get_symbol_name(nth_as_lisp_object(cons, 2)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(cons, 2)));
    /* lisp_object_free(cons); */
  }

  {
    lisp_object * cons;
    const char * s = " (a b c . d) ";
    ASSERT_INT_EQUAL(12, lisp_object_parse(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_lisp_object(cons, 0)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_lisp_object(cons, 1)));
    ASSERT_STRING_EQUAL("c", get_symbol_name(nth_as_lisp_object(cons, 2)));
    ASSERT_STRING_EQUAL("d", get_symbol_name(nth_cdr(cons, 2)));
    /* lisp_object_free(cons); */
  }

  {
    lisp_object * cons;
    const char * s = " ((a)) ";
    ASSERT_INT_EQUAL(6, lisp_object_parse(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(CAR(CAR(cons))));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(cons, 0)));
    /* lisp_object_free(cons); */
  }

  {
    lisp_object * cons;
    const char * s = " ((a b)) ";
    ASSERT_INT_EQUAL(8, lisp_object_parse(s, &cons));
    lisp_object * car = CAR(cons);
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_lisp_object(car, 0)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_lisp_object(car, 1)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(car, 1)));
    /* lisp_object_free(cons); */
  }

  {
    lisp_object * cons;
    const char * s = " (a (b c)) ";
    ASSERT_INT_EQUAL(10, lisp_object_parse(s, &cons));
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_lisp_object(cons, 0)));
    lisp_object * cdr = CDR(cons);
    lisp_object * cadr = CAR(cdr);
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_lisp_object(cadr, 0)));
    ASSERT_STRING_EQUAL("c", get_symbol_name(nth_as_lisp_object(cadr, 1)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(cadr, 1)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_cdr(cdr, 0)));
    /* lisp_object_free(cons); */
  }

  {
    lisp_object * cons;
    const char * s = " (if nil (quote abc) (quote def)) ";
    ASSERT_INT_EQUAL(33, lisp_object_parse(s, &cons));
    lisp_object_describe(cons);
    ASSERT_STRING_EQUAL("if", get_symbol_name(nth_as_lisp_object(cons, 0)));
    ASSERT_STRING_EQUAL("nil", get_symbol_name(nth_as_lisp_object(cons, 1)));
    ASSERT_STRING_EQUAL("quote", get_symbol_name(CAR(nth_as_lisp_object(cons, 2))));
    ASSERT_STRING_EQUAL("quote", get_symbol_name(CAR(nth_as_lisp_object(cons, 3))));
  }

  {
    lisp_object * cons;
    const char * s = " (let ((a 1) (b 2)) a b) ";
    ASSERT_INT_EQUAL(24, lisp_object_parse(s, &cons));
    lisp_object_describe(cons);
    ASSERT_STRING_EQUAL("let", get_symbol_name(nth_as_lisp_object(cons, 0)));
    ASSERT_STRING_EQUAL("a", get_symbol_name(CAR(CAR(nth_as_lisp_object(cons, 1)))));
    ASSERT_STRING_EQUAL("b", get_symbol_name(CAR(CAR(CDR(nth_as_lisp_object(cons, 1))))));
    ASSERT_STRING_EQUAL("a", get_symbol_name(nth_as_lisp_object(cons, 2)));
    ASSERT_STRING_EQUAL("b", get_symbol_name(nth_as_lisp_object(cons, 3)));
  }
}

TEST_CASE(test_lisp_object_parse_atom)
{
  {
    lisp_object * atom;
    ASSERT_INT_EQUAL(0, lisp_object_parse_atom("", &atom));
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM, atom->type);
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM_SYMBOL, atom->sub_type);
    ASSERT_STRING_EQUAL("nil", get_symbol_name(atom));
    /* lisp_object_free(atom); */
  }

  {
    lisp_object * atom;
    ASSERT_INT_EQUAL(1, lisp_object_parse_atom(" ", &atom));
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM, atom->type);
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM_SYMBOL, atom->sub_type);
    ASSERT_STRING_EQUAL("nil", get_symbol_name(atom));
    /* lisp_object_free(atom); */
  }

  {
    lisp_object * atom;
    ASSERT_INT_EQUAL(3, lisp_object_parse_atom("nil", &atom));
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM, atom->type);
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM_SYMBOL, atom->sub_type);
    ASSERT_STRING_EQUAL("nil", get_symbol_name(atom));
    /* lisp_object_free(atom); */
  }

  {
    lisp_object * atom;
    ASSERT_INT_EQUAL(1, lisp_object_parse_atom("a", &atom));
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM, atom->type);
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM_SYMBOL, atom->sub_type);
    ASSERT_STRING_EQUAL("a", get_symbol_name(atom));
    /* lisp_object_free(atom); */
  }

  {
    lisp_object * atom;
    ASSERT_INT_EQUAL(3, lisp_object_parse_atom(" a1", &atom));
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM, atom->type);
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM_SYMBOL, atom->sub_type);
    ASSERT_STRING_EQUAL("a1", get_symbol_name(atom));
    /* lisp_object_free(atom); */
  }

  {
    lisp_object * atom;
    ASSERT_INT_EQUAL(3, lisp_object_parse_atom(" a1 b2", &atom));
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM, atom->type);
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM_SYMBOL, atom->sub_type);
    ASSERT_STRING_EQUAL("a1", get_symbol_name(atom));
    /* lisp_object_free(atom); */
  }

  {
    lisp_object * atom;
    ASSERT_INT_EQUAL(3, lisp_object_parse_atom(" a1(", &atom));
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM, atom->type);
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM_SYMBOL, atom->sub_type);
    ASSERT_STRING_EQUAL("a1", get_symbol_name(atom));
    /* lisp_object_free(atom); */
  }

  {
    lisp_object * atom;
    ASSERT_INT_EQUAL(3, lisp_object_parse_atom(" a1)", &atom));
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM, atom->type);
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM_SYMBOL, atom->sub_type);
    ASSERT_STRING_EQUAL("a1", get_symbol_name(atom));
    /* lisp_object_free(atom); */
  }

  {
    lisp_object * atom;
    ASSERT_INT_EQUAL(0, lisp_object_parse_atom("(", &atom));
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM, atom->type);
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM_SYMBOL, atom->sub_type);
    ASSERT_STRING_EQUAL("nil", get_symbol_name(atom));
    /* lisp_object_free(atom); */
  }

  {
    lisp_object * atom;
    ASSERT_INT_EQUAL(1, lisp_object_parse_atom(" (", &atom));
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM, atom->type);
    ASSERT_INT_EQUAL(LISP_OBJECT_ATOM_SYMBOL, atom->sub_type);
    ASSERT_STRING_EQUAL("nil", get_symbol_name(atom));
    /* lisp_object_free(atom); */
  }
}
