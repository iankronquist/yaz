CC=clang
CFLAGS= -O0 -Wall -Wpedantic -g

all: yaz

yaz: build
	$(CC) yaz.c build/lexer.o build/parser.o -o build/bin/yaz  $(CFLAGS)

panic:
	$(CC) -c panic.c -o build/panic.o $(CFLAGS)

lexer:  build token_list panic
	$(CC) -c lexer.c -o build/lexer.o $(CFLAGS)

token_list: build
	$(CC) -c token_list.c -o build/token_list.o $(CFLAGS)

lexer_tests: lexer token_list build
	$(CC) lexer_tests.c build/panic.o build/token_list.o build/lexer.o \
	  -o tests/bin/lexer_tests $(CFLAGS)
	$(CC) token_list_tests.c build/token_list.o -o tests/bin/token_list_tests \
	  $(CFLAGS)

ast: build lexer
	$(CC) -c ast.c build/lexer.o -o build/ast.o $(CFLAGS)

ast_tests: ast build
	$(CC) ast_tests.c build/ast.o -o tests/bin/ast_tests \
	  $(CFLAGS)

parser:  build panic
	$(CC) -c parser.c build/panic.o -o build/parser.o $(CFLAGS)

parser_tests: parser
	$(CC) parser_tests.c build/parser.o -o tests/bin/parser_tests \
	  $(CFLAGS)

tests: ast_tests lexer_tests parser_tests

build:
	mkdir build
	mkdir -p tests/bin

clean:
	rm -rf build
	rm -f yaz
	rm -rf tests/bin
