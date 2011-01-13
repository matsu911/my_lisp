/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include <string.h>
#include <stdlib.h>
#include "atom.h"
#include "unittest.h"

LISP_BOOL is_alpha_char(const char c)
{
  if(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'))
    return LISP_TRUE;
  else
    return LISP_FALSE;
}

TEST_CASE(test_is_alpha_char)
{
  ASSERT_FALSE(is_alpha_char('a' - 1));
  ASSERT_TRUE(is_alpha_char('a'));
  ASSERT_TRUE(is_alpha_char('b'));
  ASSERT_TRUE(is_alpha_char('y'));
  ASSERT_TRUE(is_alpha_char('z'));
  ASSERT_FALSE(is_alpha_char('z' + 1));

  ASSERT_FALSE(is_alpha_char('A' - 1));
  ASSERT_TRUE(is_alpha_char('A'));
  ASSERT_TRUE(is_alpha_char('B'));
  ASSERT_TRUE(is_alpha_char('Y'));
  ASSERT_TRUE(is_alpha_char('Z'));
  ASSERT_FALSE(is_alpha_char('Z' + 1));
}

LISP_BOOL is_number_char(const char c)
{
  if('0' <= c && c <= '9')
    return LISP_TRUE;
  else
    return LISP_FALSE;
}

TEST_CASE(test_is_number_char)
{
  ASSERT_FALSE(is_number_char('0' - 1));
  ASSERT_TRUE(is_number_char('0'));
  ASSERT_TRUE(is_number_char('1'));
  ASSERT_TRUE(is_number_char('2'));
  ASSERT_TRUE(is_number_char('3'));
  ASSERT_TRUE(is_number_char('4'));
  ASSERT_TRUE(is_number_char('5'));
  ASSERT_TRUE(is_number_char('6'));
  ASSERT_TRUE(is_number_char('7'));
  ASSERT_TRUE(is_number_char('8'));
  ASSERT_TRUE(is_number_char('9'));
  ASSERT_FALSE(is_number_char('9' + 1));
}

LISP_BOOL is_number_string(const char * str)
{
  char * p = str;
  while(*p != 0)
  {
    if(LISP_FALSE == is_number_char(*p))
      return LISP_FALSE;
    ++p;
  }

  if(p == str)
    return LISP_FALSE;
  else
    return LISP_TRUE;
}

TEST_CASE(test_is_number_string)
{
  ASSERT_FALSE(is_number_string(""));
  ASSERT_TRUE(is_number_string("0"));
  ASSERT_TRUE(is_number_string("01"));
  ASSERT_TRUE(is_number_string("012"));
  ASSERT_TRUE(is_number_string("0123456789"));
  ASSERT_FALSE(is_number_string("0123456789a"));
  ASSERT_FALSE(is_number_string("a"));
  ASSERT_FALSE(is_number_string("a0123456789"));
}

/* int parse_Atom(const char * str, Atom * atom) */
/* { */
/*   char * p = str; */
/*   while(p == ' ' && */
/* } */

/* TEST_CASE(test_parse_Atom) */
/* { */
/*   { */
/*     const char * s = "abc"; */
/*     const char * tmp = (char*)malloc(sizeof(char) * strlen(s)); */
/*     ASSERT_TRUE(strcmp("", tmp) == 0); */
/*     if(tmp != NULL) */
/*       free((void*)tmp); */
/*   } */
/* } */
