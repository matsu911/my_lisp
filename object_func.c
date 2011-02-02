/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include <stdio.h>
#include <stdlib.h>
#include "object_func.h"
#include "cons_func.h"
#include "atom.h"
#include "stringutils.h"

/* void my_free(void* p) */
/* { */
/*   /\* printf("%p\n", p); *\/ */
/*   free(p); */
/* } */

/* void lisp_object_free(lisp_object * object) */
/* { */
/*   if(object == NULL) return; */

/*   if(object->type == LISP_OBJECT_ATOM) */
/*   { */
/*     if(object->atom != NULL && object->sub_type == LISP_OBJECT_ATOM_SYMBOL) */
/*       lisp_symbol_free((lisp_symbol*)object->atom); */
/*   } */
/*   else if(object->type == LISP_OBJECT_CONS) */
/*   { */
/*     if(object->car != NULL) */
/*     { */
/*       lisp_object_free(object->car); */
/*     } */

/*     if(object->cdr != NULL) */
/*     { */
/*       lisp_object_free(object->cdr); */
/*     } */
/*   } */

/*   my_free((void*)object); */
/* } */

void lisp_object_describe(lisp_object * object)
{
  if(object == NULL)
  {
    printf("object is NULL\n");
    return;
  }

  printf("OBJECT: %p\n", object);

  switch(object->type)
  {
    case LISP_OBJECT_ATOM:
      printf("TYPE: ATOM\n");
      break;
    case LISP_OBJECT_CONS:
      printf("TYPE: CONS\n");
      break;
  }

  switch(object->sub_type)
  {
    case LISP_OBJECT_ATOM_NONE:
      printf("SUB_TYPE: NONE\n");
      break;
    case LISP_OBJECT_ATOM_SYMBOL:
      printf("SUB_TYPE: SYMBOL\n");
      break;
    case LISP_OBJECT_ATOM_CHAR:
      printf("SUB_TYPE: CHAR\n");
      break;
    case LISP_OBJECT_ATOM_INTEGER:
      printf("SUB_TYPE: INTEGER\n");
      break;
    case LISP_OBJECT_ATOM_STRING:
      printf("SUB_TYPE: STRING\n");
      break;
  }

  if(CAR(object) && CAR(object)->atom && is_lisp_symbol_nil((lisp_symbol*)CAR(object)->atom))
    printf("CAR: NIL\n");
  else
    printf("CAR: %p\n", CAR(object));

  if(CDR(object) && CDR(object)->atom && is_lisp_symbol_nil((lisp_symbol*)CDR(object)->atom))
    printf("CDR: NIL\n");
  else
    printf("CDR: %p\n", CDR(object));

  printf("ATOM: %p\n", object->atom);

  if(object->type == LISP_OBJECT_ATOM)
  {
    switch(object->sub_type)
    {
      case LISP_OBJECT_ATOM_NONE:
        printf("VALUE: NONE\n");
        break;
      case LISP_OBJECT_ATOM_SYMBOL:
        printf("VALUE: %s\n", get_symbol_name(object));
        break;
      case LISP_OBJECT_ATOM_CHAR:
        printf("VALUE: %c\n", *(char*)object->atom);
        break;
      case LISP_OBJECT_ATOM_INTEGER:
        printf("VALUE: %d\n", *(int*)object->atom);
        break;
      case LISP_OBJECT_ATOM_STRING:
        printf("VALUE: \"%s\"\n", (char*)object->atom);
        break;
    }
  }

}
