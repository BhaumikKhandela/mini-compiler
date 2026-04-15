#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

typedef struct SymbolTable SymbolTable;

SymbolTable *symtab_create(void);
void symtab_destroy(SymbolTable *t);
int symtab_add(SymbolTable *t, const char *name);
int symtab_exists(SymbolTable *t, const char *name);

#endif
