/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include "unittest.h"

boolean is_all_passed(Test_Result * result)
{
  if(result->num_failed == 0)
    return TRUE;
  else
    return FALSE;
}

void print_test_result(Test_Result * result)
{
  int num_test = result->num_passed + result->num_failed;
  printf("\n");
  printf("%d/%d passed\n", result->num_passed, num_test);
  if(result->num_failed > 0)
    printf("%d/%d failed\n", result->num_failed, num_test);
}
