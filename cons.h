/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef __CONS_H__
#define __CONS_H__

typedef struct _Cons
{
  void * car;
  void * cdr;
} Cons;

Cons * new_cons();

#endif /* __CONS_H__ */
