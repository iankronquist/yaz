#!/bin/bash

for file in $(ls tests/bin); do
	if valgrind --error-exitcode=0 tests/bin/$file; then
		echo "Error: $file leaking memory"
	fi
	tests/bin/$file
done

if valgrind --error-exitcode=0 build/yazparser test.yaz; then 
	echo "Error: build/yazparser leaking memory"
fi
