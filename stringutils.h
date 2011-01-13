/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef __STRINGUTILS_H__
#define __STRINGUTILS_H__

#include "type.h"

LISP_BOOL is_begin_list_char(const char c);

LISP_BOOL is_end_list_char(const char c);

LISP_BOOL is_white_space_char(const char c);

LISP_BOOL is_alpha_char(const char c);

LISP_BOOL is_number_char(const char c);

LISP_BOOL is_number_string(const char * str);

#endif /* __STRINGUTILS_H__ */