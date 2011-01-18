/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef __OBJECT_H__
#define __OBJECT_H__

typedef enum _LISP_OBJECT_TYPE
{
  LISP_OBJECT_ATOM,
  LISP_OBJECT_CONS,
} LISP_OBJECT_TYPE;

typedef enum _LISP_OBJECT_ATOM_TYPE
{
  LISP_OBJECT_ATOM_NONE,
  LISP_OBJECT_ATOM_SYMBOL,
  LISP_OBJECT_ATOM_CHAR,
  LISP_OBJECT_ATOM_INTEGER,
  LISP_OBJECT_ATOM_STRING,
} LISP_OBJECT_ATOM_TYPE;

typedef struct _lisp_object
{
  LISP_OBJECT_TYPE type;
  LISP_OBJECT_ATOM_TYPE sub_type;
  void * car;
  void * cdr;
  void * atom;
} lisp_object;

#endif /* __OBJECT_H__ */
