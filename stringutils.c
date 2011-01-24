/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include "stringutils.h"
#include "unittest.h"
#include <string.h>

boolean is_end_string_char(const char c)
{
  if(c == '\0')
    return TRUE;
  else
    return FALSE;
}

TEST_CASE(test_is_end_string_char)
{
  ASSERT_FALSE(is_end_string_char(' '));
  ASSERT_TRUE(is_end_string_char('\0'));
  ASSERT_TRUE(is_end_string_char(0));
}

boolean is_begin_list_char(const char c)
{
  if(c == '(')
    return TRUE;
  else
    return FALSE;
}

TEST_CASE(test_is_begin_list_char)
{
  ASSERT_TRUE(is_begin_list_char('('));
  ASSERT_FALSE(is_begin_list_char(')'));
  ASSERT_FALSE(is_begin_list_char(' '));
  ASSERT_FALSE(is_begin_list_char('\0'));
}

boolean is_end_list_char(const char c)
{
  if(c == ')')
    return TRUE;
  else
    return FALSE;
}

TEST_CASE(test_is_end_list_char)
{
  ASSERT_FALSE(is_end_list_char('('));
  ASSERT_TRUE(is_end_list_char(')'));
  ASSERT_FALSE(is_end_list_char(' '));
  ASSERT_FALSE(is_end_list_char('\0'));
}

boolean is_cons_dot_char(const char c)
{
  if(c == '.')
    return TRUE;
  else
    return FALSE;
}

TEST_CASE(test_is_cons_dot_char)
{
  ASSERT_TRUE(is_cons_dot_char('.'));
  ASSERT_FALSE(is_cons_dot_char(' '));
  ASSERT_FALSE(is_cons_dot_char('('));
  ASSERT_FALSE(is_cons_dot_char(')'));
}

boolean is_white_space_char(const char c)
{
  if(c == ' ' || c == '\t' || c == '\n' || c == '\r')
    return TRUE;
  else
    return FALSE;
}

TEST_CASE(test_is_white_space_char)
{
  ASSERT_TRUE(is_white_space_char(' '));
  ASSERT_TRUE(is_white_space_char('\t'));
  ASSERT_TRUE(is_white_space_char('\n'));
  ASSERT_TRUE(is_white_space_char('\r'));
  ASSERT_FALSE(is_white_space_char('\0'));
}

boolean is_alpha_char(const char c)
{
  if(('a' <= c && c <= 'z') || ('A' <= c && c <= 'Z'))
    return TRUE;
  else
    return FALSE;
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

boolean is_number_char(const char c)
{
  if('0' <= c && c <= '9')
    return TRUE;
  else
    return FALSE;
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

boolean is_number_string(const char * str)
{
  const char * p = str;
  while(*p != 0)
  {
    if(FALSE == is_number_char(*p))
      return FALSE;
    ++p;
  }

  if(p == str)
    return FALSE;
  else
    return TRUE;
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

int skip_chars_while(const char_match_predicate pred, const char * str)
{
  const char * p = str;
  while(pred(*p) && !is_end_string_char(*p)) ++p;
  return p - str;
}

TEST_CASE(test_skip_chars_while)
{
  ASSERT_INT_EQUAL(0, skip_chars_while(is_white_space_char, ""));
  ASSERT_INT_EQUAL(1, skip_chars_while(is_white_space_char, " "));
  ASSERT_INT_EQUAL(1, skip_chars_while(is_white_space_char, " 1"));
  ASSERT_INT_EQUAL(1, skip_chars_while(is_white_space_char, " 1 b"));
  ASSERT_INT_EQUAL(3, skip_chars_while(is_white_space_char, "   1 b"));
  ASSERT_INT_EQUAL(0, skip_chars_while(is_white_space_char, "1 b"));
}

int skip_chars_while_not(const char_match_predicate pred, const char * str)
{
  const char * p = str;
  while(!pred(*p) && !is_end_string_char(*p)) ++p;
  return p - str;
}

TEST_CASE(test_skip_chars_while_not)
{
  ASSERT_INT_EQUAL(0, skip_chars_while_not(is_white_space_char, ""));
  ASSERT_INT_EQUAL(0, skip_chars_while_not(is_white_space_char, " "));
  ASSERT_INT_EQUAL(0, skip_chars_while_not(is_white_space_char, " 1"));
  ASSERT_INT_EQUAL(0, skip_chars_while_not(is_white_space_char, " 1 b"));
  ASSERT_INT_EQUAL(0, skip_chars_while_not(is_white_space_char, "   1 b"));
  ASSERT_INT_EQUAL(1, skip_chars_while_not(is_white_space_char, "1 b"));
}

char next_char_not(const char_match_predicate pred, const char * str)
{
  const char * p = str;
  while(pred(*p) && !is_end_string_char(*p)) ++p;
  return *p;
}

TEST_CASE(test_next_char_while_not)
{
  ASSERT_INT_EQUAL('\0', next_char_not(is_white_space_char, ""));
  ASSERT_INT_EQUAL('\0', next_char_not(is_white_space_char, " "));
  ASSERT_INT_EQUAL('a', next_char_not(is_white_space_char, " a"));
}

char * allocate_string(const char * s)
{
  if(s == NULL) return NULL;
  int size = strlen(s);
  char * ret = (char*)malloc(sizeof(char) * (size + 1));
  strcpy(ret, s);
  return ret;
}

TEST_CASE(test_allocate_string)
{
  {
    const char * s = "";
    const char * tmp = allocate_string(s);
    ASSERT_STRING_EQUAL("", tmp);
    free((void*)tmp);
  }

  {
    const char * s = "abc";
    const char * tmp = allocate_string(s);
    ASSERT_STRING_EQUAL("abc", tmp);
    free((void*)tmp);
  }
}
