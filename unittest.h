/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef __UNITTEST_H__
#define __UNITTEST_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "type.h"

#define ASSERT_TRUE(expression)                 \
  {                                             \
    if((expression) == LISP_TRUE)               \
    {                                           \
      ++result->num_passed;                     \
      fprintf(stdout, ".");                     \
    }                                           \
    else                                        \
    {                                           \
      ++result->num_failed;                     \
      fprintf(stdout, "\nfailed at %s:%d\n",    \
              __FILE__,  __LINE__);             \
    }                                           \
  }                                             \

#define ASSERT_FALSE(expression) ASSERT_TRUE(!expression)

#define ASSERT_INT_EQAUL(expected, value)           \
  {                                                 \
    if(expected == value)                           \
    {                                               \
      ++result->num_passed;                         \
      fprintf(stdout, ".");                         \
    }                                               \
    else                                            \
    {                                               \
      ++result->num_failed;                         \
      fprintf(stdout, "\nfailed at %s:%d\n",        \
              __FILE__,  __LINE__);                 \
      fprintf(stdout, " expected %d, but got %d\n", \
              expected, value);                     \
    }                                               \
  }                                                 \

#define ASSERT_STRING_EQUAL(expected, value)        \
  {                                                 \
    if(strcmp(expected, value) == 0)                \
    {                                               \
      ++result->num_passed;                         \
      fprintf(stdout, ".");                         \
    }                                               \
    else                                            \
    {                                               \
      ++result->num_failed;                         \
      fprintf(stdout, "\nfailed at %s:%d\n",        \
              __FILE__,  __LINE__);                 \
      fprintf(stdout, " expected %s, but got %s\n", \
              expected, value);                     \
    }                                               \
  }                                                 \

#define TEST_CASE(test_name)                    \
  void test_name(Test_Result * result)          \

typedef struct _Test_Result
{
  int num_passed;
  int num_failed;
} Test_Result;

LISP_BOOL is_all_passed(Test_Result * result);

void print_test_result(Test_Result * result);

#define REGISTER_TEST_CASE(test_name)                   \
  {                                                     \
    results = (Test_Result**)                           \
      realloc((void*)results,                           \
              sizeof(Test_Result*) * ++num_test_cases); \
                                                        \
    Test_Result * result =                              \
      (Test_Result*)malloc(sizeof(Test_Result));        \
    result->num_passed = 0;                             \
    result->num_failed = 0;                             \
    test_name(result);                                  \
    results[num_test_cases - 1] = result;               \
  }                                                     \


#define UNITTEST_MAIN_BEGIN()                   \
  int num_test_cases = 0;                       \
  Test_Result ** results = NULL;                \

#define UNITTEST_MAIN_END()                             \
  int num_passed = 0;                                   \
  int num_failed = 0;                                   \
  int i = 0;                                            \
  for(;i<num_test_cases;++i)                            \
  {                                                     \
    if(results[i] != NULL)                              \
    {                                                   \
      if(is_all_passed(results[i]) == LISP_TRUE)        \
        ++num_passed;                                   \
      else                                              \
        ++num_failed;                                   \
      free((void*)results[i]);                          \
    }                                                   \
  }                                                     \
                                                        \
  int num_test = num_passed + num_failed;               \
  printf("\n");                                         \
  printf("%d/%d passed\n", num_passed, num_test);       \
  if(num_failed > 0)                                    \
    printf("%d/%d failed\n", num_failed, num_test);     \
  return 0;                                             \

#endif /* __UNITTEST_H__ */ 
