/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include <stdlib.h>
#include <gc.h>
#include "variable.h"
#include "stringutils.h"
#include "unittest.h"

lisp_variable * lisp_variable_allocate(const char * name, 
                                       lisp_object * value, 
                                       lisp_object * function)
{
  lisp_variable * variable = (lisp_variable*)GC_MALLOC_ATOMIC(sizeof(lisp_variable));

  variable->name          = allocate_string(name);
  variable->value_slot    = value;
  variable->function_slot = function;

  return variable;
}

/* void lisp_variable_free(lisp_variable * variable) */
/* { */
/*   if(variable == NULL) */
/*     return; */

/*   if(variable->name) */
/*     free((void*)variable->name); */

/*   free(variable); */
/* } */

TEST_CASE(test_lisp_variable)
{
  lisp_variable * v = lisp_variable_allocate("a", NULL, NULL);
  ASSERT_STRING_EQUAL("a", v->name);
  ASSERT_NULL(v->value_slot);
  ASSERT_NULL(v->function_slot);
  /* lisp_variable_free(v); */
}
