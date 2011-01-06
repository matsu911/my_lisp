/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include "unittest.h"

int main()
{
  UNITTEST_MAIN_BEGIN();

  REGISTER_TEST_CASE(test_func1);
  REGISTER_TEST_CASE(test_func1);

  UNITTEST_MAIN_END();
}

