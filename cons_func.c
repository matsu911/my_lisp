/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include <gc.h>
#include "cons_func.h"
#include "object_func.h"
#include "symbol.h"
#include "unittest.h"
#include "stringutils.h"

lisp_object * lisp_object_allocate_cons()
{
  lisp_object * cons = (lisp_object*)GC_MALLOC_ATOMIC(sizeof(lisp_object));

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

lisp_object * car_as_lisp_object(const lisp_object * cons)
{
  return cons->car;
}

lisp_object * cdr_as_lisp_object(const lisp_object * cons)
{
  return cons->cdr;
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

  if(CAR(cons)->type == LISP_OBJECT_CONS)
  {
    printf("(");
    lisp_object_print_cons_internal(CAR(cons));
  }
  else if(car_as_lisp_object(cons)->type == LISP_OBJECT_ATOM)
  {
    const char * s = lisp_object_print_atom(CAR(cons));
    printf("%s", s);
    /* free((void*)s); */
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
    /* free((void*)s); */
    printf(")");
  }
}

void lisp_object_print_cons(const lisp_object * cons)
{
  printf("(");
  lisp_object_print_cons_internal(cons);
  /* printf(")"); */
  printf("\n");
}

void lisp_object_print(const lisp_object * obj)
{
  if(obj->type == LISP_OBJECT_CONS)
    lisp_object_print_cons(obj);
  else if(obj->type == LISP_OBJECT_ATOM)
    printf("%s\n", lisp_object_print_atom(obj));
}

