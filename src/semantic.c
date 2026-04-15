#include "semantic.h"
#include "symbol_table.h"
#include <stdio.h>

static int errors;
static FILE *out;

static void check_expr(const Expr *e, SymbolTable *t){
    if(!e) return;
    if(e->type==EXPR_VAR && !symtab_exists(t,e->data.var_name)){
        fprintf(out,"Error: undeclared variable '%s'\n", e->data.var_name); errors++;
    } else if(e->type==EXPR_BINOP){
        check_expr(e->data.bin.left,t);
        check_expr(e->data.bin.right,t);
    }
}

static void check_stmt(const Stmt *s, SymbolTable *t){
    for(; s; s=s->next){
        if(s->type==STMT_ASSIGN){
            if(!symtab_exists(t,s->data.assign.var)){ fprintf(out,"Error: assignment to undeclared variable '%s'\n", s->data.assign.var); errors++; }
            check_expr(s->data.assign.expr,t);
        } else if(s->type==STMT_IF){
            check_expr(s->data.if_stmt.cond,t);
            check_stmt(s->data.if_stmt.then_branch,t);
            check_stmt(s->data.if_stmt.else_branch,t);
        } else if(s->type==STMT_WHILE){
            check_expr(s->data.while_stmt.cond,t);
            check_stmt(s->data.while_stmt.body,t);
        } else {
            check_stmt(s->data.block.stmts,t);
        }
    }
}

int semantic_check(const Program *p, const char *out_path){
    out=fopen(out_path,"w"); if(!out) return 0;
    SymbolTable*t=symtab_create(); errors=0;
    for(Decl*d=p->decls; d; d=d->next){
        if(!symtab_add(t,d->name)){ fprintf(out,"Error: redeclaration of '%s'\n",d->name); errors++; }
        else fprintf(out,"Declared: %s\n", d->name);
    }
    check_stmt(p->stmts,t);
    if(errors==0) fprintf(out,"Semantic analysis successful\n");
    else fprintf(out,"Semantic analysis failed with %d errors\n", errors);
    symtab_destroy(t); fclose(out); return errors==0;
}
