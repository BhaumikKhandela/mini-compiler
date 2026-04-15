# Mini Compiler Report

## 1. Architecture
This project implements a Toy-C mini compiler using Flex and Bison with the following pipeline:
1. Lexical analysis (`src/lexer.l`)
2. Syntax analysis with AST construction (`src/parser.y`, `src/ast.*`)
3. Semantic analysis (`src/semantic.c`, `src/symbol_table.c`)
4. Three-address code generation (`src/tac.c`)
5. Basic optimization (`src/optimizer.c`)
6. Target code generation (`src/codegen.c`)

## 2. Core Grammar
Supported language subset:
- Data type: `int`
- Expressions: `+ - * / < > == !=`
- Statements: assignment, `if-else`, `while`, block

Grammar highlights (Bison):
- `program -> decl_list stmt_list`
- `decl_list -> decl_list INT id_list ';' | empty`
- `stmt -> IDENT '=' expr ';' | IF '(' cond ')' stmt ELSE stmt | WHILE '(' cond ')' stmt | block`
- `expr` supports binary operators and parentheses.

Parser used: **Bison LALR(1)** parser with precedence declarations.

## 3. Chosen ISA And Instruction Subset
Target is a MIPS-like hypothetical ISA with limited registers (`R0`..`R3` used by generator):
- Data movement: `LI`, `LOAD`, `STORE`
- Arithmetic: `ADD`, `SUB`, `MUL`, `DIV`
- Comparison/set: `SLT`, `SEQ`, `SNE`
- Branch/control flow: `BEQ`, `J`, labels

## 4. Semantic Rules Implemented
- Undeclared variable usage detection
- Assignment to undeclared variable detection
- Redeclaration detection
- Type model: int-only (all expressions evaluate as int)

## 5. TAC And Optimization
TAC representation uses quadruples (`op, arg1, arg2, result`).
Optimizations implemented:
- Constant propagation (simple variable-value substitution)
- Common subexpression elimination for repeated arithmetic quads

## 6. Test Programs
Five programs are provided in `tests/`:
- `test1.toy`: arithmetic assignments
- `test2.toy`: nested expression and repeatable subexpression
- `test3.toy`: if-else with comparison
- `test4.toy`: while loop accumulation
- `test5.toy`: mixed if-else and loop

For each test, outputs are produced in `outputs/<test>/`:
- `tokens.txt`
- `ast.txt`
- `semantic.txt`
- `tac.txt`
- `optimized_tac.txt`
- `assembly.s`

## 7. Build And Run
```bash
make
./compiler tests/test1.toy outputs/test1
make run-tests
```

## 8. Limitations
- No function support
- No nested scopes in symbol table (single global scope)
- Register allocation is minimal and fixed-register based
