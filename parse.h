/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef __PARSE_H__
#define __PARSE_H__

#include "object.h"

int lisp_object_parse(const char * str, lisp_object ** cons);

#endif /* __PARSE_H__ */
