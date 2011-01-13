/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include <string.h>
#include <stdlib.h>
#include "chomp.h"
#include "unittest.h"

void chomp(const char * str)
{
  size_t size = strlen(str);
  if(size == 0) return;
  char * p = (char*)str + size;
  while(str != --p)
  {
    if(*p == '\r' || *p == '\n' || *p == ' ')
      *p = '\0';
    else
      return;
  }
}

TEST_CASE(test_chomp)
{
  {
    const char * s = "";
    const char * tmp = (char*)malloc(sizeof(char) * strlen(s));
    ASSERT_STRING_EQUAL("", tmp);
    if(tmp != NULL)
      free((void*)tmp);
  }

  {
    const char * s = "\n";
    const char * tmp = (char*)malloc(sizeof(char) * strlen(s));
    ASSERT_STRING_EQUAL("", tmp);
    if(tmp != NULL)
      free((void*)tmp);
  }

  {
    const char * s = " ";
    const char * tmp = (char*)malloc(sizeof(char) * strlen(s));
    ASSERT_STRING_EQUAL("", tmp);
    if(tmp != NULL)
      free((void*)tmp);
  }

  {
    const char * s = "\r";
    const char * tmp = (char*)malloc(sizeof(char) * strlen(s));
    ASSERT_STRING_EQUAL("", tmp);
    if(tmp != NULL)
      free((void*)tmp);
  }

  {
    const char * s = "\r\n";
    const char * tmp = (char*)malloc(sizeof(char) * strlen(s));
    ASSERT_STRING_EQUAL("", tmp);
    if(tmp != NULL)
      free((void*)tmp);
  }
}
