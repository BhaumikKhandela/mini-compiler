%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ast.h"

extern int yylex(void);
extern FILE *yyin;
void yyerror(const char *s);
Program parsed_program;
%}

%union {
    int ival;
    char *sval;
    Expr *expr;
    Stmt *stmt;
    Decl *decl;
}

%token <sval> IDENT
%token <ival> NUMBER
%token INT IF ELSE WHILE
%token EQ NEQ LT GT

%type <decl> decl_list id_list
%type <stmt> stmt stmt_list block
%type <expr> expr cond

%left EQ NEQ LT GT
%left '+' '-'
%left '*' '/'

%%
program : decl_list stmt_list { parsed_program.decls = $1; parsed_program.stmts = $2; }
;

decl_list : decl_list INT id_list ';' { $$ = $1; Decl *t=$3; while(t){ $$ = append_decl($$, make_decl(strdup(t->name))); t=t->next; } }
          | /* empty */ { $$ = NULL; }
;

id_list : id_list ',' IDENT { $$ = append_decl($1, make_decl($3)); }
        | IDENT { $$ = make_decl($1); }
;

stmt_list : stmt_list stmt { $$ = append_stmt($1, $2); }
          | /* empty */ { $$ = NULL; }
;

stmt : IDENT '=' expr ';' { $$ = make_assign_stmt($1, $3); }
     | IF '(' cond ')' stmt ELSE stmt { $$ = make_if_stmt($3, $5, $7); }
     | WHILE '(' cond ')' stmt { $$ = make_while_stmt($3, $5); }
     | block { $$ = $1; }
;

block : '{' stmt_list '}' { $$ = make_block_stmt($2); }
;

cond : expr { $$ = $1; }
;

expr : expr '+' expr { $$ = make_bin_expr(OP_ADD,$1,$3); }
     | expr '-' expr { $$ = make_bin_expr(OP_SUB,$1,$3); }
     | expr '*' expr { $$ = make_bin_expr(OP_MUL,$1,$3); }
     | expr '/' expr { $$ = make_bin_expr(OP_DIV,$1,$3); }
     | expr LT expr  { $$ = make_bin_expr(OP_LT,$1,$3); }
     | expr GT expr  { $$ = make_bin_expr(OP_GT,$1,$3); }
     | expr EQ expr  { $$ = make_bin_expr(OP_EQ,$1,$3); }
     | expr NEQ expr { $$ = make_bin_expr(OP_NEQ,$1,$3); }
     | '(' expr ')' { $$ = $2; }
     | IDENT { $$ = make_var_expr($1); }
     | NUMBER { $$ = make_int_expr($1); }
;
%%

void yyerror(const char *s) { fprintf(stderr, "Parse error: %s\n", s); }
