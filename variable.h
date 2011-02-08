/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef __VARIABLE_H__
#define __VARIABLE_H__

#include "object.h"

typedef struct _lisp_variable
{
  const char * name;
  lisp_object * value_slot;
  lisp_object * function_slot;
} lisp_variable;

lisp_variable * lisp_variable_allocate(const char * name, lisp_object * value, lisp_object * function);

#endif /* __VARIABLE_H__ */
