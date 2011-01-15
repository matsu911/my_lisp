/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include "unittest.h"

TEST_CASE(test_chomp);
TEST_CASE(test_is_alpha_char);
TEST_CASE(test_is_number_char);
TEST_CASE(test_is_number_string);
TEST_CASE(test_is_white_space_char);
TEST_CASE(test_is_begin_list_char);
TEST_CASE(test_is_end_list_char);
TEST_CASE(test_is_end_string_char);
TEST_CASE(test_parse_list);
TEST_CASE(test_parse_Atom);

int main()
{
  UNITTEST_MAIN_BEGIN();

  REGISTER_TEST_CASE(test_chomp);
  REGISTER_TEST_CASE(test_is_alpha_char);
  REGISTER_TEST_CASE(test_is_number_char);
  REGISTER_TEST_CASE(test_is_number_string);
  REGISTER_TEST_CASE(test_is_white_space_char);
  REGISTER_TEST_CASE(test_is_begin_list_char);
  REGISTER_TEST_CASE(test_is_end_list_char);
  REGISTER_TEST_CASE(test_is_end_string_char);
  /* REGISTER_TEST_CASE(test_parse_list); */
  REGISTER_TEST_CASE(test_parse_Atom);

  UNITTEST_MAIN_END();
}

