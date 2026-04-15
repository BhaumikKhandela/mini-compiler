#include "symbol_table.h"
#include <stdlib.h>
#include <string.h>

typedef struct Symbol { char *name; struct Symbol *next; } Symbol;
struct SymbolTable { Symbol *head; };

SymbolTable *symtab_create(void){ return calloc(1,sizeof(SymbolTable)); }
void symtab_destroy(SymbolTable *t){ Symbol*s=t->head; while(s){Symbol*n=s->next; free(s->name); free(s); s=n;} free(t);} 
int symtab_exists(SymbolTable *t,const char*name){ for(Symbol*s=t->head;s;s=s->next) if(strcmp(s->name,name)==0) return 1; return 0; }
int symtab_add(SymbolTable *t,const char*name){ if(symtab_exists(t,name)) return 0; Symbol*s=calloc(1,sizeof(*s)); s->name=strdup(name); s->next=t->head; t->head=s; return 1; }
