/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef __BLOCK_TAG_H__
#define __BLOCK_TAG_H__

#include "object.h"
#include <setjmp.h>

typedef struct _lisp_block_tag
{
  const char * name;
  jmp_buf jmpbuf;
  lisp_object * value_slot;
  lisp_object * function_slot;
} lisp_block_tag;

lisp_block_tag * lisp_block_tag_allocate(const char * name);
/* lisp_variable * lisp_variable_allocate(const char * name, lisp_object * value, lisp_object * function); */

#endif /* __BLOCK_TAG_H__ */
