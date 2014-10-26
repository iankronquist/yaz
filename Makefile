CC=clang
CFLAGS= -O0 -Wall -Wpedantic -g

yaz: build
	$(CC) yaz.c build/lexer.o build/parser.o -o build/bin/yaz  $(CFLAGS)

lexer:  build
	$(CC) -c lexer.c -o build/lexer.o $(CFLAGS)

test_lexer: lexer build
	$(CC) lexer_tests.c build/lexer.o -o tests/bin/lexer_tests $(CFLAGS) 

ast: build lexer
	$(CC) -c ast.c build/lexer.o -o build/ast.o $(CFLAGS)

test_ast: ast build
	$(CC) ast_tests.c build/lexer.o build/ast.o -o tests/bin/ast_tests $(CFLAGS)

parser:  build
	$(CC) -c parser.c -o build/parser.o $(CFLAGS)

build:
	mkdir build
	mkdir -p tests/bin

tests: tree_tests hashmap_tests


clean:
	rm -rf build
	rm -f yaz
	rm -rf tests/bin
