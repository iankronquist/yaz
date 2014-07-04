CC=clang
CFLAGS=-std=c99 -O0 -Wall -Wpedantic

yaz: build
	$(CC) -c yazparser.c -o build/yazparser.c $(CFLAGS)
	$(CC) -c yazvm.c -o build/yazvm.o $(CFLAGS)
	$(CC) yaz.c build/yazparser.o build/yazvm.o -o yaz $(CFLAGS)

datastructures: build hashmap

datastructures_tests: build hashmap_tests

hashmap: build
	$(CC) -c datastructures/hashmap.c -o build/hashmap.o $(CFLAGS)

hashmap_tests: build hashmap
	$(CC) build/hashmap.o datastructures/hashmaptests.c -o build/hashmap_tests $(CFLAGS)

build:
	mkdir build

clean:
	rm -rf build
	rm -f yaz
