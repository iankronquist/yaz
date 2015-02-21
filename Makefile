CC=clang
CFLAGS= -O0 -Wall -Wpedantic -g

yaz: build
	$(CC) yaz.c build/lexer.o build/parser.o -o build/bin/yaz  $(CFLAGS)


lexer:  build token_list
	$(CC) -c lexer.c build/token_list.o -o build/lexer.o $(CFLAGS)

token_list: build
	$(CC) -c token_list.c -o build/token_list.o $(CFLAGS)

test_lexer: lexer token_list build
	$(CC) lexer_tests.c build/token_list.o build/lexer.o -o tests/bin/lexer_tests $(CFLAGS)
	$(CC) token_list_tests.c build/token_list.o -o tests/bin/token_list_tests $(CFLAGS)

ast: build lexer
	$(CC) -c ast.c build/lexer.o -o build/ast.o $(CFLAGS)

test_ast: ast build
	$(CC) ast_tests.c build/lexer.o build/ast.o -o tests/bin/ast_tests $(CFLAGS)

parser:  build
	$(CC) -c parser.c -o build/parser.o $(CFLAGS)

tests: test_ast test_lexer

build:
	mkdir build
	mkdir -p tests/bin

clean:
	rm -rf build
	rm -f yaz
	rm -rf tests/bin
