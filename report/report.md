# Mini Compiler Report

## 1. Architecture
This project implements a Toy-C mini compiler using Flex and Bison with the following pipeline:
1. Lexical analysis (`src/lexer.l`)
2. Syntax analysis with AST construction (`src/parser.y`, `src/ast.*`)
3. Semantic analysis (`src/semantic.c`, `src/symbol_table.c`)
4. Three-address code generation (`src/tac.c`)
5. Basic optimization (`src/optimizer.c`)
6. Target code generation (`src/codegen.c`)

The lexer also skips C-style preprocessor directives (e.g. `#include ...`) so toy inputs that look like C headers can still be tokenized.

## 2. Core Grammar
Supported language subset:
- Data type: `int`
- Expressions: `+ - * / < > == !=`
- Statements: declarations, assignment, `if-else`, `while`, block

Grammar highlights (Bison) in this version:
- `program -> stmt_list`
- `stmt_list -> stmt_list stmt | empty`
- Declarations:
  - `decl_stmt -> INT id_list ';'`
  - A `decl_stmt` can appear as a `stmt` (including inside `{ ... }` blocks)
- Function wrapper:
  - `main_def_stmt -> INT IDENT '(' ')' block`
  - `main_def_stmt` is treated as a top-level `stmt` whose value is its inner `block`
- Control flow:
  - `stmt -> IF '(' cond ')' stmt ELSE stmt | WHILE '(' cond ')' stmt | block`
- Expressions:
  - binary ops: `+ - * / < > == !=`
  - parentheses and atoms: `IDENT`, `NUMBER`

Parser used: **Bison LALR(1)** parser with precedence declarations.

## 3. Chosen ISA And Instruction Subset
Target is a MIPS-like hypothetical ISA generated from TAC (pseudo assembly):
- Data movement: `LI`, `LOAD`, `STORE`
- Arithmetic: `ADD`, `SUB`, `MUL`, `DIV`
- Comparison/set: `SLT`, `SEQ`, `SNE`
- Branch/control flow: `BEQ` (for TAC `IFZ`), `J` (for TAC `GOTO`), and labels

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
- `test1.toy`: nested expression and repeatable subexpression
- `test2.toy`: if-else with comparison
- `test3.toy`: while loop accumulation
- `test4.toy`: mixed if-else and loop
- `test5.toy`: `int main() { ... }` wrapper with block-scoped declarations

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
./scripts/run_tests.sh
```

