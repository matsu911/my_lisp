/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

#ifndef __SYMBOL_H__
#define __SYMBOL_H__

typedef struct _Symbol
{
  int id;
  char * name;
} Symbol;

Symbol * new_symbol();
Symbol * new_symbol_nil();
void delete_symbol(Symbol * symbol);

#endif /* __SYMBOL_H__ */
