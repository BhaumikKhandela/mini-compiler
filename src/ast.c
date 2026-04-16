#include "ast.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void print_indent(int n) { while (n--) printf("  "); }

Expr *make_int_expr(int value){ Expr *e=calloc(1,sizeof(*e)); e->type=EXPR_INT; e->data.int_val=value; return e; }
Expr *make_var_expr(char *name){ Expr *e=calloc(1,sizeof(*e)); e->type=EXPR_VAR; e->data.var_name=name; return e; }
Expr *make_bin_expr(int op, Expr *left, Expr *right){ Expr *e=calloc(1,sizeof(*e)); e->type=EXPR_BINOP; e->data.bin.op=op; e->data.bin.left=left; e->data.bin.right=right; return e; }
Stmt *make_assign_stmt(char *name, Expr *expr){ Stmt *s=calloc(1,sizeof(*s)); s->type=STMT_ASSIGN; s->data.assign.var=name; s->data.assign.expr=expr; return s; }
Stmt *make_if_stmt(Expr *cond, Stmt *then_branch, Stmt *else_branch){ Stmt *s=calloc(1,sizeof(*s)); s->type=STMT_IF; s->data.if_stmt.cond=cond; s->data.if_stmt.then_branch=then_branch; s->data.if_stmt.else_branch=else_branch; return s; }
Stmt *make_while_stmt(Expr *cond, Stmt *body){ Stmt *s=calloc(1,sizeof(*s)); s->type=STMT_WHILE; s->data.while_stmt.cond=cond; s->data.while_stmt.body=body; return s; }
Stmt *make_block_stmt(Stmt *stmts){ Stmt *s=calloc(1,sizeof(*s)); s->type=STMT_BLOCK; s->data.block.stmts=stmts; return s; }
Decl *make_decl(char *name){ Decl *d=calloc(1,sizeof(*d)); d->name=name; return d; }
Decl *append_decl(Decl *list, Decl *node){ if(!list) return node; Decl *t=list; while(t->next)t=t->next; t->next=node; return list; }
Stmt *make_decl_stmt(Decl *decls){ Stmt *s=calloc(1,sizeof(*s)); s->type=STMT_DECL; s->data.decl_stmt.decls=decls; return s; }
Stmt *append_stmt(Stmt *list, Stmt *node){ if(!list) return node; Stmt *t=list; while(t->next)t=t->next; t->next=node; return list; }

static void free_expr(Expr *e){ if(!e) return; if(e->type==EXPR_VAR) free(e->data.var_name); if(e->type==EXPR_BINOP){ free_expr(e->data.bin.left); free_expr(e->data.bin.right);} free(e);} 
static void free_decls(Decl *d){ while(d){ Decl*n=d->next; free(d->name); free(d); d=n; } }
static void free_stmt(Stmt *s){ while(s){ Stmt *n=s->next; if(s->type==STMT_ASSIGN){ free(s->data.assign.var); free_expr(s->data.assign.expr);} else if(s->type==STMT_IF){ free_expr(s->data.if_stmt.cond); free_stmt(s->data.if_stmt.then_branch); free_stmt(s->data.if_stmt.else_branch);} else if(s->type==STMT_WHILE){ free_expr(s->data.while_stmt.cond); free_stmt(s->data.while_stmt.body);} else if(s->type==STMT_BLOCK){ free_stmt(s->data.block.stmts);} else if(s->type==STMT_DECL){ free_decls(s->data.decl_stmt.decls);} free(s); s=n; } }
void free_program(Program *p){ Decl *d=p->decls; while(d){ Decl*n=d->next; free(d->name); free(d); d=n;} free_stmt(p->stmts);} 

static const char* op_str(int op){ switch(op){case OP_ADD:return "+";case OP_SUB:return "-";case OP_MUL:return "*";case OP_DIV:return "/";case OP_LT:return "<";case OP_GT:return ">";case OP_EQ:return "==";case OP_NEQ:return "!=";default:return "?";}}
static void print_expr(const Expr *e){ if(!e) return; if(e->type==EXPR_INT) printf("%d",e->data.int_val); else if(e->type==EXPR_VAR) printf("%s",e->data.var_name); else { printf("("); print_expr(e->data.bin.left); printf(" %s ",op_str(e->data.bin.op)); print_expr(e->data.bin.right); printf(")"); }}
static void print_stmts(const Stmt *s, int ind){ for(;s;s=s->next){ print_indent(ind); if(s->type==STMT_ASSIGN){ printf("assign %s = ",s->data.assign.var); print_expr(s->data.assign.expr); printf("\n"); } else if(s->type==STMT_IF){ printf("if "); print_expr(s->data.if_stmt.cond); printf("\n"); print_stmts(s->data.if_stmt.then_branch, ind+1); if(s->data.if_stmt.else_branch){ print_indent(ind); printf("else\n"); print_stmts(s->data.if_stmt.else_branch, ind+1);} } else if(s->type==STMT_WHILE){ printf("while "); print_expr(s->data.while_stmt.cond); printf("\n"); print_stmts(s->data.while_stmt.body, ind+1);} else if(s->type==STMT_DECL){ printf("decl "); for(Decl*d=s->data.decl_stmt.decls; d; d=d->next){ printf("%s", d->name); if(d->next) printf(", "); } printf("\n"); } else { printf("block\n"); print_stmts(s->data.block.stmts, ind+1);} }}
void print_ast(const Program *p){ printf("Declarations:\n"); for(Decl*d=p->decls; d; d=d->next) printf("  int %s\n", d->name); printf("Statements:\n"); print_stmts(p->stmts,1);} 
