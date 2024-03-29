/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include <stdlib.h>
#include <gc.h>
#include "lexical_scope.h"
#include "unittest.h"

lisp_lexical_scope * lisp_lexical_scope_allocate(lisp_lexical_scope * outer)
{
  lisp_lexical_scope * scope = (lisp_lexical_scope*)GC_MALLOC_ATOMIC(sizeof(lisp_lexical_scope));

  scope->outer     = outer;
  scope->variables = NULL;

  return scope;
}

boolean lisp_lexical_scope_add_variable(lisp_lexical_scope * scope, lisp_variable * variable)
{
  if(scope == NULL || variable == NULL || variable->name == NULL)
    return FALSE;

  if(scope->variables == NULL)
  {
    scope->variables = linked_list_allocate(variable);
  }
  else if(lisp_lexical_scope_lookup_local_variable(scope, variable->name))
  {
    return FALSE;
  }
  else
  {
    linked_list_append(scope->variables, variable);
  }

  return TRUE;
}

static boolean lisp_variable_name_match(void * a, void * b)
{
  return a && b && strcmp(((lisp_variable*)a)->name, ((lisp_variable*)b)->name) == 0;
}

lisp_variable * lisp_lexical_scope_lookup_local_variable(lisp_lexical_scope * scope, const char * name)
{
  if(scope == NULL)
    return NULL;

  lisp_variable tmp;
  tmp.name = name;
  linked_list * l = linked_list_find(scope->variables, &tmp, lisp_variable_name_match);
  if(l)
    return l->data;
  else
    return NULL;
}

lisp_variable * lisp_lexical_scope_lookup_variable(lisp_lexical_scope * scope, const char * name)
{
  if(scope == NULL)
    return NULL;

  lisp_variable * ret = lisp_lexical_scope_lookup_local_variable(scope, name);
  if(ret)
    return ret;
  else
    return lisp_lexical_scope_lookup_variable(scope->outer, name);
}

boolean lisp_lexical_scope_add_block_tag(lisp_lexical_scope * scope, lisp_block_tag * block_tag)
{
  if(scope == NULL || block_tag == NULL || block_tag->name == NULL)
    return FALSE;

  if(scope->variables == NULL)
  {
    scope->variables = linked_list_allocate(block_tag);
  }
  else if(lisp_lexical_scope_lookup_local_block_tag(scope, block_tag->name))
  {
    return FALSE;
  }
  else
  {
    linked_list_append(scope->variables, block_tag);
  }

  return TRUE;
}

static boolean lisp_block_tag_name_match(void * a, void * b)
{
  return a && b && strcmp(((lisp_block_tag*)a)->name, ((lisp_block_tag*)b)->name) == 0;
}

lisp_block_tag * lisp_lexical_scope_lookup_local_block_tag(lisp_lexical_scope * scope, const char * name)
{
  if(scope == NULL)
    return NULL;

  lisp_block_tag tmp;
  tmp.name = name;
  linked_list * l = linked_list_find(scope->variables, &tmp, lisp_block_tag_name_match);
  if(l)
    return l->data;
  else
    return NULL;
}

lisp_block_tag * lisp_lexical_scope_lookup_block_tag(lisp_lexical_scope * scope, const char * name)
{
  if(scope == NULL)
    return NULL;

  lisp_block_tag * ret = lisp_lexical_scope_lookup_local_block_tag(scope, name);
  if(ret)
    return ret;
  else
    return lisp_lexical_scope_lookup_block_tag(scope->outer, name);
}

TEST_CASE(test_lisp_lexical_scope)
{
  lisp_lexical_scope * scope = lisp_lexical_scope_allocate(NULL);

  ASSERT_NULL(scope->outer);
  ASSERT_NULL(scope->variables);

  ASSERT_NULL(lisp_lexical_scope_lookup_variable(scope, ""));
  ASSERT_NULL(lisp_lexical_scope_lookup_variable(scope, "a"));
  ASSERT_NULL(lisp_lexical_scope_lookup_variable(scope, "b"));
  
  lisp_variable * v_a = lisp_variable_allocate("a", NULL, NULL);
  ASSERT_TRUE(lisp_lexical_scope_add_variable(scope, v_a));

  ASSERT_NULL(lisp_lexical_scope_lookup_variable(scope, ""));
  ASSERT_NULL(lisp_lexical_scope_lookup_variable(scope, "b"));

  {
    lisp_variable * v = lisp_lexical_scope_lookup_variable(scope, "a");
    ASSERT_POINTER_EQUAL(v_a, v);
    ASSERT_STRING_EQUAL("a", v->name);
  }

  ASSERT_FALSE(lisp_lexical_scope_add_variable(scope, lisp_variable_allocate("a", NULL, NULL)));

  {
    lisp_variable * v = lisp_lexical_scope_lookup_variable(scope, "a");
    ASSERT_POINTER_EQUAL(v_a, v);
    ASSERT_STRING_EQUAL("a", v->name);
  }

  lisp_lexical_scope * nested_scope = lisp_lexical_scope_allocate(scope);

  {
    lisp_variable * v = lisp_lexical_scope_lookup_variable(nested_scope, "a");
    ASSERT_POINTER_EQUAL(v_a, v);
    ASSERT_STRING_EQUAL("a", v->name);
  }

  lisp_variable * vv_a = lisp_variable_allocate("a", NULL, NULL);
  ASSERT_TRUE(lisp_lexical_scope_add_variable(nested_scope, vv_a));

  {
    lisp_variable * v = lisp_lexical_scope_lookup_variable(nested_scope, "a");
    ASSERT_POINTER_EQUAL(vv_a, v);
    ASSERT_STRING_EQUAL("a", v->name);
  }

  lisp_variable * vv_b = lisp_variable_allocate("b", NULL, NULL);
  ASSERT_TRUE(lisp_lexical_scope_add_variable(nested_scope, vv_b));

  {
    lisp_variable * v = lisp_lexical_scope_lookup_variable(nested_scope, "b");
    ASSERT_POINTER_EQUAL(vv_b, v);
    ASSERT_STRING_EQUAL("b", v->name);
  }

}
