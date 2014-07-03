CC=clang
CFLAGS=-std=c99 -O0 -Wall -Wpedantic

yaz: build
	$(CC) -c yazparser.c -o build/yazparser.c
	$(CC) -c yazvm.c -o build/yazvm.o
	$(CC) yaz.c build/yazparser.o build/yazvm.o -o yaz

tests: build yaz

build:
	mkdir build

clean:
	rm -rf build
	rm -f yaz
