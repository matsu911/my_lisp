/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef __ATOM_H__
#define __ATOM_H__

typedef struct _Atom
{
  enum { ATOM_NIL,
         ATOM_INTEGER,
         ATOM_SYMBOL,
  } type;

  void * ptr;
} Atom;

int parse_Atom(const char * str, Atom * atom);

#endif /* __ATOM_H__ */
