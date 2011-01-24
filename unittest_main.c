/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include "unittest.h"

TEST_CASE(test_chomp);
TEST_CASE(test_is_alpha_char);
TEST_CASE(test_is_number_char);
TEST_CASE(test_is_number_string);
TEST_CASE(test_is_white_space_char);
TEST_CASE(test_is_cons_dot_char);
TEST_CASE(test_is_begin_list_char);
TEST_CASE(test_is_end_list_char);
TEST_CASE(test_is_end_string_char);
TEST_CASE(test_parse_list);
TEST_CASE(test_lisp_object_parse_atom);
TEST_CASE(test_lisp_object_parse_cons);
TEST_CASE(test_skip_chars_while);
TEST_CASE(test_skip_chars_while_not);
TEST_CASE(test_next_char_while_not);
TEST_CASE(test_allocate_string);
TEST_CASE(test_lisp_object_parse_cons_internal);
TEST_CASE(test_eval);
TEST_CASE(test_is_lisp_symbol_nil);
TEST_CASE(test_lisp_object_atom_with_string);
TEST_CASE(test_linked_list);
TEST_CASE(test_lisp_variable);
TEST_CASE(test_lisp_lexical_scope);

int main()
{
  UNITTEST_MAIN_BEGIN();

  REGISTER_TEST_CASE(test_chomp);
  REGISTER_TEST_CASE(test_is_alpha_char);
  REGISTER_TEST_CASE(test_is_number_char);
  REGISTER_TEST_CASE(test_is_number_string);
  REGISTER_TEST_CASE(test_is_cons_dot_char);
  REGISTER_TEST_CASE(test_is_white_space_char);
  REGISTER_TEST_CASE(test_is_begin_list_char);
  REGISTER_TEST_CASE(test_is_end_list_char);
  REGISTER_TEST_CASE(test_is_end_string_char);
  /* REGISTER_TEST_CASE(test_parse_list); */
  REGISTER_TEST_CASE(test_lisp_object_parse_atom);
  REGISTER_TEST_CASE(test_lisp_object_parse_cons);
  REGISTER_TEST_CASE(test_skip_chars_while);
  REGISTER_TEST_CASE(test_skip_chars_while_not);
  REGISTER_TEST_CASE(test_next_char_while_not);
  REGISTER_TEST_CASE(test_allocate_string);
  REGISTER_TEST_CASE(test_lisp_object_parse_cons_internal);
  REGISTER_TEST_CASE(test_eval);
  REGISTER_TEST_CASE(test_is_lisp_symbol_nil);
  REGISTER_TEST_CASE(test_lisp_object_atom_with_string);
  REGISTER_TEST_CASE(test_linked_list);
  REGISTER_TEST_CASE(test_lisp_variable);
  REGISTER_TEST_CASE(test_lisp_lexical_scope);

  UNITTEST_MAIN_END();
}

