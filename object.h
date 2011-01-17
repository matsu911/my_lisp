/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef __OBJECT_H__
#define __OBJECT_H__

enum OBJECT_TYPE
{
  OBJECT_ATOM,
  OBJECT_CONS,
};

typedef struct _Object
{
  enum OBJECT_TYPE type;
  void * ptr;
} Object;

/* Object * new_Object(); */
/* Object * new_Object_with_Atom(Atom * atom); */
/* Object * new_Object_with_Cons(Cons * cons); */
/* void delete_Object(Object * object); */

#endif /* __OBJECT_H__ */
