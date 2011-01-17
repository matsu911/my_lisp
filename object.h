/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef __OBJECT_H__
#define __OBJECT_H__

enum OBJECT_TYPE
{
  OBJECT_ATOM,
  OBJECT_CONS,
};

enum OBJECT_ATOM_TYPE
{
  OBJECT_ATOM_NONE,
  OBJECT_ATOM_SYMBOL,
  OBJECT_ATOM_CHAR,
  OBJECT_ATOM_INTEGER,
  OBJECT_ATOM_STRING,
};

typedef struct _Object
{
  enum OBJECT_TYPE type;
  enum OBJECT_ATOM_TYPE sub_type;
  void * car;
  void * cdr;
  void * atom;
} Object;

#endif /* __OBJECT_H__ */
