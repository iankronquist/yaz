CC=clang
CFLAGS=-std=c99 -O0 -Wall -Wpedantic -g

yaz: build hashmap tree
	$(CC) yazparser.c build/hashmap.o build/tree.o -o build/yazparser  $(CFLAGS)
	#$(CC) -c yazparser.c -o build/yazparser.o $(CFLAGS)
	#$(CC) -c yazvm.c -o build/yazvm.o $(CFLAGS)
	#$(CC) yaz.c build/yazparser.o build/yazvm.o -o yaz $(CFLAGS)

datastructures: build hashmap tree

datastructures_tests: build hashmap_tests tree_tests

hashmap: build
	$(CC) -c datastructures/hashmap.c -o build/hashmap.o $(CFLAGS)

hashmap_tests: hashmap
	$(CC) build/hashmap.o tests/hashmaptests.c -o tests/bin/hashmap_tests $(CFLAGS)

tree: build
	$(CC) -c datastructures/tree.c -o build/tree.o $(CFLAGS)

tree_tests: tree
	$(CC) tests/treetests.c build/tree.o -o tests/bin/tree_tests $(CFLAGS)

build:
	mkdir build
	mkdir tests/bin

tests: tree_tests hashmap_tests

docs:
	doxygen Doxyfile

clean:
	rm -rf build
	rm -f yaz
	rm -rf docs
	rm -rf tests/bin/*
