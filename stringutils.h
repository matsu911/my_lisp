/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef __STRINGUTILS_H__
#define __STRINGUTILS_H__

#include "type.h"

boolean is_end_string_char(const char c);

boolean is_begin_list_char(const char c);

boolean is_end_list_char(const char c);

boolean is_cons_dot_char(const char c);

boolean is_white_space_char(const char c);

boolean is_alpha_char(const char c);

boolean is_number_char(const char c);

boolean is_number_string(const char * str);

typedef boolean (*char_match_predicate)(const char);

int skip_chars_while(const char_match_predicate pred, const char * str);

int skip_chars_while_not(const char_match_predicate pred, const char * str);

char next_char_not(const char_match_predicate pred, const char * str);

char * allocate_string(const char * s);

#endif /* __STRINGUTILS_H__ */
