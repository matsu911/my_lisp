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

#define CAR(cons) ((lisp_object*)((lisp_object*)cons)->car)
#define CDR(cons) ((lisp_object*)((lisp_object*)cons)->cdr)

#define NTHCDR_0(cons) CDR(cons)
#define NTHCDR_1(cons) CDR(NTHCDR_0(cons))
#define NTHCDR_2(cons) CDR(NTHCDR_1(cons))
#define NTHCDR_3(cons) CDR(NTHCDR_2(cons))
#define NTHCDR_4(cons) CDR(NTHCDR_3(cons))
#define NTHCDR_5(cons) CDR(NTHCDR_4(cons))
#define NTHCDR_6(cons) CDR(NTHCDR_5(cons))
#define NTHCDR_7(cons) CDR(NTHCDR_6(cons))
#define NTHCDR_8(cons) CDR(NTHCDR_7(cons))
#define NTHCDR_9(cons) CDR(NTHCDR_8(cons))

#define NTH_0(cons) CAR(cons)
#define NTH_1(cons) CAR(NTHCDR_0(cons))
#define NTH_2(cons) CAR(NTHCDR_1(cons))
#define NTH_3(cons) CAR(NTHCDR_2(cons))
#define NTH_4(cons) CAR(NTHCDR_3(cons))
#define NTH_5(cons) CAR(NTHCDR_4(cons))
#define NTH_6(cons) CAR(NTHCDR_5(cons))
#define NTH_7(cons) CAR(NTHCDR_6(cons))
#define NTH_8(cons) CAR(NTHCDR_7(cons))
#define NTH_9(cons) CAR(NTHCDR_8(cons))

#endif /* __OBJECT_H__ */
