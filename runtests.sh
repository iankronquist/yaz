#!/bin/bash

make tests

for file in $(find tests/bin -type f -maxdepth 1); do
	echo "Testing " $file
	if valgrind --error-exitcode=0 $file $TEST_ARGS; then
		echo "Error:" $file "leaking memory" 1>&2
	fi
	tests/bin/$file
done

for file in $(find build/bin -type f -maxdepth 1); do
	echo "Testing " $file
	if valgrind --error-exitcode=0 $file $ARGS; then 
		echo "Error: leaking memory"
	fi
done
