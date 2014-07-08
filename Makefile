CC=clang
CFLAGS=-std=c99 -O0 -Wall -Wpedantic -g

yaz: build hashmap tree
	$(CC) yazparser.c build/hashmap.o build/tree.o -o build/yazparser  $(CFLAGS)
	#$(CC) -c yazparser.c -o build/yazparser.o $(CFLAGS)
	#$(CC) -c yazvm.c -o build/yazvm.o $(CFLAGS)
	#$(CC) yaz.c build/yazparser.o build/yazvm.o -o yaz $(CFLAGS)

datastructures: build hashmap

datastructures_tests: build hashmap_tests

hashmap: build
	$(CC) -c datastructures/hashmap.c -o build/hashmap.o $(CFLAGS)

hashmap_tests: tests hashmap
	$(CC) build/hashmap.o datastructures/hashmaptests.c -o tests/hashmap_tests $(CFLAGS)

tree: build
	$(CC) -c datastructures/tree.c -o build/tree.o $(CFLAGS)

tree_tests: tests tree
	$(CC) datastructures/treetests.c -o tests/tree.o $(CFLAGS)

build:
	mkdir build

tests:
	mkdir tests

docs:
	doxygen Doxyfile

clean:
	rm -rf build
	rm -f yaz
	rm -rf tests
	rm -rf docs
