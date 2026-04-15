#ifndef AST_H
#define AST_H

typedef enum { EXPR_INT, EXPR_VAR, EXPR_BINOP } ExprType;
typedef enum { STMT_ASSIGN, STMT_IF, STMT_WHILE, STMT_BLOCK } StmtType;

typedef struct Expr Expr;
typedef struct Stmt Stmt;
typedef struct Decl Decl;

enum BinOp { OP_ADD, OP_SUB, OP_MUL, OP_DIV, OP_LT, OP_GT, OP_EQ, OP_NEQ };

struct Expr {
    ExprType type;
    union {
        int int_val;
        char *var_name;
        struct { int op; Expr *left; Expr *right; } bin;
    } data;
};

struct Stmt {
    StmtType type;
    Stmt *next;
    union {
        struct { char *var; Expr *expr; } assign;
        struct { Expr *cond; Stmt *then_branch; Stmt *else_branch; } if_stmt;
        struct { Expr *cond; Stmt *body; } while_stmt;
        struct { Stmt *stmts; } block;
    } data;
};

struct Decl {
    char *name;
    Decl *next;
};

typedef struct {
    Decl *decls;
    Stmt *stmts;
} Program;

Expr *make_int_expr(int value);
Expr *make_var_expr(char *name);
Expr *make_bin_expr(int op, Expr *left, Expr *right);
Stmt *make_assign_stmt(char *name, Expr *expr);
Stmt *make_if_stmt(Expr *cond, Stmt *then_branch, Stmt *else_branch);
Stmt *make_while_stmt(Expr *cond, Stmt *body);
Stmt *make_block_stmt(Stmt *stmts);
Decl *make_decl(char *name);
Decl *append_decl(Decl *list, Decl *node);
Stmt *append_stmt(Stmt *list, Stmt *node);
void free_program(Program *program);
void print_ast(const Program *program);

#endif
