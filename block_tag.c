/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include <stdlib.h>
#include <gc.h>
#include "block_tag.h"
#include "stringutils.h"
#include "unittest.h"

lisp_block_tag * lisp_block_tag_allocate(const char * name)
{
  lisp_block_tag * block_tag = (lisp_block_tag*)GC_MALLOC_ATOMIC(sizeof(lisp_block_tag));

  block_tag->name          = allocate_string(name);
  block_tag->value_slot    = lisp_symbol_nil();
  block_tag->function_slot = lisp_symbol_nil();

  return block_tag;
}

TEST_CASE(test_lisp_block_tag)
{
  lisp_block_tag * t = lisp_block_tag_allocate("a");
  ASSERT_STRING_EQUAL("a", t->name);
  ASSERT_TRUE(is_lisp_symbol_nil(t->value_slot));
  ASSERT_TRUE(is_lisp_symbol_nil(t->function_slot));
}
