/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include "unittest.h"

int main()
{
  UNITTEST_MAIN_BEGIN();

  REGISTER_TEST_CASE(test_chomp);
  REGISTER_TEST_CASE(test_is_alpha_char);
  REGISTER_TEST_CASE(test_is_number_char);
  REGISTER_TEST_CASE(test_is_number_string);

  UNITTEST_MAIN_END();
}

