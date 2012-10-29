/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include <setjmp.h>
#include "cons_func.h"
#include "object_func.h"
#include "lexical_scope.h"
#include "symbol.h"
#include "parse.h"
#include "unittest.h"

lisp_object * eval(const lisp_object * cons, lisp_lexical_scope * lexical_scope, lisp_lexical_scope * block_tags);

typedef struct _special_operator
{
  const char * name;
  lisp_object * (*func)(const lisp_object * cons, 
                        lisp_lexical_scope * lexical_scope,
                        lisp_lexical_scope * block_tags);
} special_operator;

typedef struct _defined_function
{
  const char * name;
  lisp_object * (*func)(const lisp_object * cons, 
                        lisp_lexical_scope * lexical_scope,
                        lisp_lexical_scope * block_tags);
} defined_function;

lisp_object * lisp_defun_cons(const lisp_object * cons, 
                              lisp_lexical_scope * lexical_scope,
                              lisp_lexical_scope * block_tags)
{
  lisp_object * cdr = CDR(cons);
  /* lisp_object_print(cdr); */
  return lisp_object_cons(eval(CAR(cdr), lexical_scope, block_tags), 
                          eval(CAR(CDR(cdr)), lexical_scope, block_tags));
}

lisp_object * lisp_special_operator_setq(const lisp_object * cons, 
                                         lisp_lexical_scope * lexical_scope,
                                         lisp_lexical_scope * block_tags)
{
  lisp_object * cdr = CDR(cons);

  lisp_object * var = CAR(cdr);
  
  /* lisp_object_describe(CAR(CDR(cdr))); */
  lisp_object * ret = eval(CAR(CDR(cdr)), lexical_scope, block_tags);
  /* lisp_object_describe(ret); */

  if(lisp_lexical_scope_lookup_local_variable(lexical_scope, get_symbol_name(var)))
  {
    // TODO: remove
  }

  if(!lisp_lexical_scope_add_variable(lexical_scope, 
                                      lisp_variable_allocate(get_symbol_name(var), ret, NULL)))
    fprintf(stderr, "failed to add variable \"%s\" to %p\n", get_symbol_name(var), lexical_scope);

  return ret;
}

lisp_object * lisp_special_operator_quote(const lisp_object * cons, 
                                          lisp_lexical_scope * lexical_scope,
                                          lisp_lexical_scope * block_tags)
{
  return CAR(CDR(cons));
}

lisp_object * lisp_special_operator_if(const lisp_object * cons, 
                                       lisp_lexical_scope * lexical_scope,
                                       lisp_lexical_scope * block_tags)
{
  lisp_object * condition = eval(CAR(CDR(cons)), lexical_scope, block_tags);
  if(is_lisp_symbol_nil(condition->atom))
  {
    return eval(CAR(CDR(CDR(CDR(cons)))), lexical_scope, block_tags);
  }
  else
  {
    return eval(CAR(CDR(CDR(cons))), lexical_scope, block_tags);
  }
}

static lisp_object * sequential_eval_and_return_last(const lisp_object * cons, 
                                                     lisp_lexical_scope * lexical_scope,
                                                     lisp_lexical_scope * block_tags)
{
  lisp_object * tmp = cons;
  lisp_object * ret = lisp_symbol_nil();
  while(tmp && CAR(tmp) && !is_lisp_symbol_nil(tmp))
  {
    ret = eval(CAR(tmp), lexical_scope, block_tags);
    tmp = CDR(tmp);
  }
  return ret;
}

lisp_object * lisp_special_operator_progn(const lisp_object * cons, 
                                          lisp_lexical_scope * lexical_scope,
                                          lisp_lexical_scope * block_tags)
{
  return sequential_eval_and_return_last(CDR(cons), lexical_scope, block_tags);
}

lisp_object * lisp_special_operator_let(const lisp_object * cons, 
                                        lisp_lexical_scope * lexical_scope,
                                        lisp_lexical_scope * block_tags)
{
  lisp_lexical_scope * new_lexical_scope = lisp_lexical_scope_allocate(lexical_scope);

  lisp_object * variables = CAR(CDR(cons));
  while(variables && CAR(variables))
  {
    lisp_object * form = CAR(variables);
    lisp_object * var = CAR(form);
    lisp_object * val = eval(CAR(CDR(form)), lexical_scope, block_tags);
    boolean ret = lisp_lexical_scope_add_variable(new_lexical_scope, 
                                                  lisp_variable_allocate(get_symbol_name(var),
                                                                         val,
                                                                         (void*)lisp_symbol_nil()));
    if(!ret)
      fprintf(stderr, "failed to add variable \"%s\" to %p\n", get_symbol_name(var), new_lexical_scope);
    variables = CDR(variables);
  }

  /* lisp_object_describe(CDR(CDR(cons))); */

  return sequential_eval_and_return_last(CDR(CDR(cons)), new_lexical_scope, block_tags);
}

lisp_object * lisp_special_operator_block(const lisp_object * cons,
                                          lisp_lexical_scope * lexical_scope,
                                          lisp_lexical_scope * block_tags)
{
  lisp_lexical_scope * new_block_tags = lisp_lexical_scope_allocate(block_tags);

  lisp_object * block_tag = CAR(CDR(cons));

  lisp_block_tag * new_block_tag = lisp_block_tag_allocate(get_symbol_name(block_tag));
  lisp_lexical_scope_add_block_tag(new_block_tags, new_block_tag);

  if(setjmp(new_block_tag->jmpbuf))
  {
    lisp_block_tag * v = lisp_lexical_scope_lookup_block_tag(new_block_tags, get_symbol_name(block_tag));
    if(v == NULL) return NULL;
    return v->value_slot;
  }

  return sequential_eval_and_return_last(CDR(CDR(cons)), lexical_scope, new_block_tags);
}

lisp_object * lisp_special_operator_return_from(const lisp_object * cons,
                                                lisp_lexical_scope * lexical_scope,
                                                lisp_lexical_scope * block_tags)
{
  lisp_object * block_tag = CAR(CDR(cons));

  lisp_block_tag * tag = lisp_lexical_scope_lookup_block_tag(block_tags, get_symbol_name(block_tag));

  lisp_object * ret = eval(CAR(CDR(CDR(cons))), lexical_scope, block_tags);

  if(tag)
  {
    tag->value_slot = ret;
    printf("return-from %s %p\n",get_symbol_name(block_tag), &tag->jmpbuf);
    longjmp(tag->jmpbuf, -1);
    return NULL;
  }

  return ret;
}

lisp_object * lisp_defun_symbol_name(const lisp_object * cons, 
                                     lisp_lexical_scope * lexical_scope,
                                     lisp_lexical_scope * block_tags)
{
  lisp_object * cdr = CDR(cons);

  lisp_object * ret = eval(CAR(cdr), lexical_scope, block_tags);

  return lisp_object_atom_with_string(get_symbol_name(ret));
}

special_operator SPECIAL_OPERATORS[] =
{
  { "setq", lisp_special_operator_setq },
  { "quote", lisp_special_operator_quote },
  { "if", lisp_special_operator_if },
  { "progn", lisp_special_operator_progn },
  { "let", lisp_special_operator_let },
  { "block", lisp_special_operator_block },
  { "return-from", lisp_special_operator_return_from },
};

defined_function DEFINED_FUNCTIONS[] =
{
  { "cons", lisp_defun_cons },
  { "symbol-name", lisp_defun_symbol_name },
};

#define FOREACH_ARRAY(param, head, type) for(type * param = (head), * __end__ = (head) + sizeof((head)) / sizeof(type); param != __end__; ++param)

lisp_object * eval(const lisp_object * object, 
                   lisp_lexical_scope * lexical_scope, 
                   lisp_lexical_scope * block_tags)
{
  if(object == NULL) return NULL;

  if(object->type == LISP_OBJECT_ATOM)
  {
    if(object->sub_type == LISP_OBJECT_ATOM_SYMBOL)
    {
      if(is_lisp_symbol_nil(object->atom))
        return (lisp_object*)object;
      lisp_variable * v = lisp_lexical_scope_lookup_variable(lexical_scope, get_symbol_name(object));
      if(v)
        return v->value_slot;
    }
    else if(object->sub_type == LISP_OBJECT_ATOM_INTEGER)
    {
      return (lisp_object*)object;
    }

    return NULL;
  }

  const char * car_symbol_name = get_symbol_name((lisp_object*)object->car);
  if(car_symbol_name == NULL) return NULL;

  FOREACH_ARRAY(p, SPECIAL_OPERATORS, special_operator)
  {
    if(strcmp(p->name, car_symbol_name) == 0)
      return p->func(object, lexical_scope, block_tags);
  }

  FOREACH_ARRAY(p, DEFINED_FUNCTIONS, defined_function)
  {
    if(strcmp(p->name, car_symbol_name) == 0)
      return p->func(object, lexical_scope, block_tags);
  }

  return NULL;
}

TEST_CASE(test_eval)
{
  /* { */
  /*   lisp_lexical_scope * lexical_scope = lisp_lexical_scope_allocate(NULL); */
  /*   const char * s = "nil"; */
  /*   lisp_object * cons; */
  /*   lisp_object_parse(s, &cons); */
  /*   lisp_object_describe(cons); */
  /* } */

  /* /\* { *\/ */
  /* /\*   lisp_lexical_scope * lexical_scope = lisp_lexical_scope_allocate(NULL); *\/ */
  /* /\*   ASSERT_TRUE(lisp_lexical_scope_add_variable(lexical_scope, lisp_variable_allocate("a1", (void*)lisp_object_atom_with_string("string"), (void*)lisp_symbol_nil()))); *\/ */
  /* /\*   lisp_object * atom; *\/ */
  /* /\*   ASSERT_INT_EQUAL(3, lisp_object_parse_atom(" a1)", &atom)); *\/ */
  /* /\*   ASSERT_INT_EQUAL(LISP_OBJECT_ATOM, atom->type); *\/ */
  /* /\*   ASSERT_INT_EQUAL(LISP_OBJECT_ATOM_SYMBOL, atom->sub_type); *\/ */
  /* /\*   ASSERT_STRING_EQUAL("a1", get_symbol_name(atom)); *\/ */
  /* /\*   lisp_object * obj = eval(atom, lexical_scope); *\/ */
  /* /\*   printf("%s\n", lisp_object_print_atom(obj)); *\/ */
  /* /\*   /\\* lisp_object_print(obj); *\\/ *\/ */
  /* /\*   lisp_object_free(atom); *\/ */
  /* /\* } *\/ */

  /* /\* { *\/ */
  /* /\*   lisp_lexical_scope * lexical_scope = lisp_lexical_scope_allocate(NULL); *\/ */
  /* /\*   const char * s = "1"; *\/ */
  /* /\*   lisp_object * obj; *\/ */
  /* /\*   lisp_object_parse_atom(s, &obj); *\/ */
  /* /\*   /\\* lisp_object_parse(s, &obj); *\\/ *\/ */
  /* /\*   printf("%s\n", lisp_object_print_atom(obj)); *\/ */
  /* /\*   /\\* lisp_object * obj = eval(cons, lexical_scope); *\\/ *\/ */
  /* /\*   /\\* lisp_object_print(obj); *\\/ *\/ */
  /* /\*   lisp_object_free(obj); *\/ */
  /* /\*   /\\* lisp_object_free(cons); *\\/ *\/ */
  /* /\* } *\/ */

  /* /\* { *\/ */
  /* /\*   lisp_lexical_scope * lexical_scope = lisp_lexical_scope_allocate(NULL); *\/ */
  /* /\*   const char * s = "(quote a)"; *\/ */
  /* /\*   lisp_object * cons; *\/ */
  /* /\*   lisp_object_parse(s, &cons); *\/ */
  /* /\*   { *\/ */
  /* /\*     lisp_object * obj = eval(cons, lexical_scope); *\/ */
  /* /\*     /\\* lisp_object_describe(obj); *\\/ *\/ */
  /* /\*     printf("%s\n", lisp_object_print_atom(obj)); *\/ */
  /* /\*     /\\* lisp_object_free(obj); *\\/ *\/ */
  /* /\*   } *\/ */
  /* /\*   /\\* lisp_object_free(cons); *\\/ *\/ */
  /* /\* } *\/ */

  /* /\* { *\/ */
  /* /\*   lisp_lexical_scope * lexical_scope = lisp_lexical_scope_allocate(NULL); *\/ */
  /* /\*   const char * s = "(quote (a))"; *\/ */
  /* /\*   lisp_object * cons; *\/ */
  /* /\*   lisp_object_parse(s, &cons); *\/ */
  /* /\*   { *\/ */
  /* /\*     lisp_object * obj = eval(cons, lexical_scope); *\/ */
  /* /\*     lisp_object_describe(obj); *\/ */
  /* /\*     /\\* printf("%s\n", lisp_object_print_atom(obj)); *\\/ *\/ */
  /* /\*     /\\* lisp_object_free(obj); *\\/ *\/ */
  /* /\*   } *\/ */
  /* /\*   /\\* lisp_object_free(cons); *\\/ *\/ */
  /* /\* } *\/ */

  /* { */
  /*   lisp_lexical_scope * lexical_scope = lisp_lexical_scope_allocate(NULL); */
  /*   const char * s = "(setq a 1)"; */
  /*   lisp_object * cons; */
  /*   lisp_object_parse(s, &cons); */
  /*   { */
  /*     lisp_object * obj = eval(cons, lexical_scope, NULL); */
  /*     lisp_object_describe(obj); */
  /*     /\* printf("%s\n", lisp_object_print_atom(obj)); *\/ */
  /*     /\* lisp_object_free(obj); *\/ */
  /*   } */
  /*   /\* lisp_object_free(cons); *\/ */

  /*   lisp_object * atom; */
  /*   lisp_object_parse("a", &atom); */
  /*   { */
  /*     lisp_object * obj = eval(atom, lexical_scope, NULL); */
  /*     lisp_object_describe(obj); */
  /*     printf("%s\n", lisp_object_print_atom(obj)); */
  /*   } */
  /* } */

  /* { */
  /*   lisp_lexical_scope * lexical_scope = lisp_lexical_scope_allocate(NULL); */
  /*   const char * s = "(symbol-name (quote abc))"; */
  /*   lisp_object * cons; */
  /*   lisp_object_parse(s, &cons); */
  /*   { */
  /*     lisp_object * obj = eval(cons, lexical_scope, NULL); */
  /*     lisp_object_describe(obj); */
  /*     /\* printf("%s\n", lisp_object_print_atom(obj)); *\/ */
  /*     /\* lisp_object_free(obj); *\/ */
  /*   } */
  /* } */

  /* { */
  /*   lisp_lexical_scope * lexical_scope = lisp_lexical_scope_allocate(NULL); */
  /*   const char * s = "(if 1 (quote abc) (quote def))"; */
  /*   lisp_object * cons; */
  /*   lisp_object_parse(s, &cons); */
  /*   lisp_object_print(cons); */
  /*   { */
  /*     lisp_object * obj = eval(cons, lexical_scope, NULL); */
  /*     lisp_object_describe(obj); */
  /*     /\* printf("%s\n", lisp_object_print_atom(obj)); *\/ */
  /*     /\* lisp_object_free(obj); *\/ */
  /*   } */
  /* } */

  /* { */
  /*   lisp_lexical_scope * lexical_scope = lisp_lexical_scope_allocate(NULL); */
  /*   const char * s = "(if nil (quote abc) (quote def))"; */
  /*   lisp_object * cons; */
  /*   lisp_object_parse(s, &cons); */
  /*   lisp_object_describe(cons); */
  /*   lisp_object_print(cons); */
  /*   { */
  /*     lisp_object * obj = eval(cons, lexical_scope, NULL); */
  /*     lisp_object_describe(obj); */
  /*     printf("%s\n", lisp_object_print_atom(obj)); */
  /*     /\* lisp_object_free(obj); *\/ */
  /*   } */
  /* } */

  /* { */
  /*   lisp_lexical_scope * lexical_scope = lisp_lexical_scope_allocate(NULL); */
  /*   const char * s = "(progn 1 2 3)"; */
  /*   lisp_object * cons; */
  /*   lisp_object_parse(s, &cons); */
  /*   /\* lisp_object_describe(cons); *\/ */
  /*   /\* lisp_object_print(cons); *\/ */
  /*   { */
  /*     lisp_object * obj = eval(cons, lexical_scope, NULL); */
  /*     lisp_object_describe(obj); */
  /*     lisp_object_print(cons); */
  /*     printf("%s\n", lisp_object_print_atom(obj)); */
  /*     /\* lisp_object_free(obj); *\/ */
  /*   } */
  /* } */

  /* { */
  /*   lisp_lexical_scope * lexical_scope = lisp_lexical_scope_allocate(NULL); */
  /*   const char * s = "(let ((a 1) (b 2)) a b)"; */
  /*   lisp_object * cons; */
  /*   lisp_object_parse(s, &cons); */
  /*   /\* lisp_object_describe(cons); *\/ */
  /*   /\* lisp_object_print(cons); *\/ */
  /*   { */
  /*     lisp_object * obj = eval(cons, lexical_scope, NULL); */
  /*     lisp_object_describe(obj); */
  /*     lisp_object_print(cons); */
  /*     printf("%s\n", lisp_object_print_atom(obj)); */
  /*     /\* lisp_object_free(obj); *\/ */
  /*   } */
  /* } */

  /* { */
  /*   lisp_lexical_scope * lexical_scope = lisp_lexical_scope_allocate(NULL); */
  /*   const char * s = "(let ((a 1) (b 2)) (let ((a 11) (b 12)) a b))"; */
  /*   lisp_object * cons; */
  /*   lisp_object_parse(s, &cons); */
  /*   /\* lisp_object_describe(cons); *\/ */
  /*   /\* lisp_object_print(cons); *\/ */
  /*   { */
  /*     lisp_object * obj = eval(cons, lexical_scope, NULL); */
  /*     lisp_object_describe(obj); */
  /*     lisp_object_print(cons); */
  /*     printf("%s\n", lisp_object_print_atom(obj)); */
  /*     /\* lisp_object_free(obj); *\/ */
  /*   } */
  /* } */

  {
    lisp_lexical_scope * lexical_scope = lisp_lexical_scope_allocate(NULL);
    const char * s = "(block b (block a (cons 1 2) (return-from b 3) (return-from a 1) (cons 2 3)) 2)";
    lisp_object * cons;
    lisp_object_parse(s, &cons);
    /* lisp_object_describe(cons); */
    /* lisp_object_print(cons); */
    {
      lisp_object * obj = eval(cons, lexical_scope, NULL);
      lisp_object_describe(obj);
      lisp_object_print(obj);
      lisp_object_print(cons);
      /* lisp_object_free(obj); */
    }
  }

  /* { */
  /*   lisp_lexical_scope * lexical_scope = lisp_lexical_scope_allocate(NULL); */
  /*   const char * s = "(cons 1 2)"; */
  /*   lisp_object * cons; */
  /*   lisp_object_parse(s, &cons); */
  /*   lisp_object_print(cons); */
  /*   lisp_object * obj = eval(cons, lexical_scope); */
  /*   lisp_object_print(obj); */
  /*   /\* lisp_object_free(obj); *\/ */
  /*   /\* lisp_object_free(cons); *\/ */
  /* } */

  /* { */
  /*   lisp_lexical_scope * lexical_scope = lisp_lexical_scope_allocate(NULL); */
  /*   ASSERT_TRUE(lisp_lexical_scope_add_variable(lexical_scope, lisp_variable_allocate("a", lisp_object_atom_with_string("string"), lisp_symbol_nil()))); */
  /*   const char * s = "(cons a b)"; */
  /*   lisp_object * cons; */
  /*   lisp_object_parse(s, &cons); */
  /*   lisp_object_print(cons); */
  /*   lisp_object * obj = eval(cons, lexical_scope); */
  /*   lisp_object_print(obj); */
  /*   /\* lisp_object_free(obj); *\/ */
  /*   /\* lisp_object_free(cons); *\/ */
  /* } */
}
