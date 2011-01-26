/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef __OBJECT_FUNC_H__
#define __OBJECT_FUNC_H__

#include "object.h"

/* void lisp_object_free(lisp_object * object); */

int lisp_object_parse(const char * str, lisp_object ** object);

void lisp_object_describe(lisp_object * object);

#endif /* __OBJECT_FUNC_H__ */
