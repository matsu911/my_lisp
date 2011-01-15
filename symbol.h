/* -*- mode: c; tab-width: 2; indent-tabs-mode: nil; c-basic-offset: 2 -*- */

typedef struct _Symbol
{
  int id;
  char * name;
} Symbol;

Symbol * new_symbol();
Symbol * new_symbol_nil();
void delete_symbol(Symbol * symbol);
