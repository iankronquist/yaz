CC=clang
CFLAGS= -O0 -Wall -Wpedantic -g

all: yaz

yaz: build
	$(CC) src/yaz.c build/lexer.o build/parser.o -o build/bin/yaz  $(CFLAGS)

panic:
	$(CC) -c src/panic.c -o build/panic.o $(CFLAGS)

lexer:  build token_list panic
	$(CC) -c src/lexer.c -o build/lexer.o $(CFLAGS)

token_list: build
	$(CC) -c src/token_list.c -o build/token_list.o $(CFLAGS)

lexer_tests: lexer token_list build
	$(CC) tests/src/lexer_tests.c build/panic.o build/token_list.o \
	  build/lexer.o -o tests/bin/lexer_tests $(CFLAGS)
	$(CC) tests/src/token_list_tests.c build/token_list.o \
	  -o tests/bin/token_list_tests $(CFLAGS)

ast: build lexer
	$(CC) -c src/ast.c -o build/ast.o $(CFLAGS)

ast_tests: ast build
	$(CC) tests/src/ast_tests.c build/ast.o -o tests/bin/ast_tests \
	  $(CFLAGS)

parser:  build panic
	$(CC) -c src/parser.c -o build/parser.o $(CFLAGS)

parser_tests: parser token_list panic ast
	$(CC) tests/src/parser_tests.c build/panic.o build/ast.o \
	  build/token_list.o build/parser.o -o tests/bin/parser_tests $(CFLAGS)

tests: ast_tests lexer_tests parser_tests

docs:
	doxygen Doxyfile

build:
	mkdir build
	mkdir -p tests/bin

clean:
	rm -rf build
	rm -f yaz
	rm -rf tests/bin
	rm -rf docs
