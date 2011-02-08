/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef __LEXICAL_SCOPE_H__
#define __LEXICAL_SCOPE_H__

#include "linked_list.h"
#include "variable.h"
#include "block_tag.h"

typedef struct _lisp_lexical_scope
{
  void * outer;
  linked_list * variables;
} lisp_lexical_scope;

lisp_lexical_scope * lisp_lexical_scope_allocate(lisp_lexical_scope * outer);

boolean lisp_lexical_scope_add_variable(lisp_lexical_scope * scope, lisp_variable * variable);
lisp_variable * lisp_lexical_scope_lookup_variable(lisp_lexical_scope * scope, const char * name);
lisp_variable * lisp_lexical_scope_lookup_local_variable(lisp_lexical_scope * scope, const char * name);

boolean lisp_lexical_scope_add_block_tag(lisp_lexical_scope * scope, lisp_block_tag * block_tag);
lisp_block_tag * lisp_lexical_scope_lookup_block_tag(lisp_lexical_scope * scope, const char * name);
lisp_block_tag * lisp_lexical_scope_lookup_local_block_tag(lisp_lexical_scope * scope, const char * name);

#endif /* __LEXICAL_SCOPE_H__ */
