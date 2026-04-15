CC=gcc
CFLAGS=-std=c11 -Wall -Wextra -g -Isrc
BISON=bison
FLEX=flex

SRC=src/main.c src/ast.c src/symbol_table.c src/semantic.c src/tac.c src/optimizer.c src/codegen.c
GEN=src/parser.tab.c src/lex.yy.c
OBJ=$(SRC:.c=.o) $(GEN:.c=.o)

all: compiler

src/parser.tab.c src/parser.tab.h: src/parser.y src/ast.h
	$(BISON) -d -o src/parser.tab.c src/parser.y

src/lex.yy.c: src/lexer.l src/parser.tab.h
	$(FLEX) -o src/lex.yy.c src/lexer.l

compiler: src/parser.tab.c src/lex.yy.c $(OBJ)
	$(CC) $(CFLAGS) -o compiler $(OBJ)

run-tests: compiler
	./scripts/run_tests.sh

clean:
	rm -f compiler src/*.o src/lex.yy.c src/parser.tab.c src/parser.tab.h

.PHONY: all clean run-tests
