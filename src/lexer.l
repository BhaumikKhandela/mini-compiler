%{
#include "ast.h"
#include "parser.tab.h"
#include <stdlib.h>
#include <string.h>
%}

%%
#[^\n]*              { /* skip preprocessor directives like #include ... */ }
"int"                 { return INT; }
"if"                  { return IF; }
"else"                { return ELSE; }
"while"               { return WHILE; }
"=="                  { return EQ; }
"!="                  { return NEQ; }
"<"                   { return LT; }
">"                   { return GT; }
[0-9]+                { yylval.ival = atoi(yytext); return NUMBER; }
[a-zA-Z_][a-zA-Z0-9_]* { yylval.sval = strdup(yytext); return IDENT; }
[ \t\r\n]+           { }
","|";"|"("|")"|"{"|"}"|"="|"+"|"-"|"*"|"/" { return yytext[0]; }
.                     { return yytext[0]; }
%%
int yywrap(void) { return 1; }
