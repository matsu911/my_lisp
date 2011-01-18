/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "symbol.h"
#include "atom.h"
#include "unittest.h"

/* lisp_object * new_atom() */
/* { */
/*   Atom *p = (Atom*)malloc(sizeof(Atom)); */
/*   p->ptr = NULL; */
/*   return p; */
/* } */

/* int * new_int(int n) */
/* { */
/*   int *p = (int*)malloc(sizeof(int)); */
/*   *p = n; */
/*   return p; */
/* } */

/* Cons * new_cons() */
/* { */
/*   Cons * p = (Cons*)malloc(sizeof(Cons)); */
/*   p->car = (void*)lisp_symbol_nil(); */
/*   p->cdr = (void*)lisp_symbol_nil(); */
/*   return p; */
/* } */

/* #define nil NULL */
#define TRUE 1
#define FALSE 0

int is_integer(char * str)
{
  char * p = str;
  while(*p != '\0')
  {
    if(*p < '0' || *p > '9')
      return FALSE;
    ++p;
  }
  return TRUE;
}

/* Atom * parse_atom(char * str) */
/* { */
/*   Atom *p = new_atom(); */

/*   if(is_integer(str) == TRUE) */
/*   { */
/*     p->ptr  = new_int(atoi(str)); */
/*     p->type = ATOM_INTEGER; */
/*   } */
  
/*   return p; */
/* } */

/* Cons * parse_list(char * str) */
/* { */
/*   Cons * cons = new_cons(); */
/*   char * p = str; */
/*   while(*p == '(') ++p; */

/*   return cons; */
/* } */

/* void parse(char * str) */
/* { */
/* } */

/* void print_cons(Cons * cons) */
/* { */
/* } */

/* void print_atom(Atom * atom) */
/* { */
/*   if(atom->type == ATOM_INTEGER) */
/*   { */
/*     printf("%d\n", *((int*)atom->ptr)); */
/*   } */
/*   else if(atom->type == ATOM_NIL) */
/*   { */
/*     printf("nil\n"); */
/*   } */
/* } */

/* int main() */
/* { */
/*   const int line_size = 256; */
/*   char line[256]; */
/*   while(1) */
/*   { */
/*     fprintf(stdout, "REPL> ", line); */
/*     fgets(line, line_size, stdin); */
/*     chomp(line); */
/*     if(strcmp(line, "exit") == 0) */
/*       break; */
/*     /\* fprintf(stdout, "%c\n", line[0]); *\/ */
/*     /\* fprintf(stdout, "%s\n", line); *\/ */
/*     Atom * atom = parse_atom(line); */
/*     print_atom(atom); */
/*   } */
/*   return 0; */
/* } */
