#!/bin/bash

make tests

exit_code=0

for file in $(find tests/bin -type f -maxdepth 1); do
	echo "Testing " $file
	if valgrind --error-exitcode=1 $file $TEST_ARGS; then
		echo "Error:" $file "leaking memory" 1>&2
		exit_code=1
	fi
	tests/bin/$file
done

for file in $(find build/bin -type f -maxdepth 1); do
	echo "Testing " $file
	if valgrind --error-exitcode=1 $file $ARGS; then
		echo "Error: leaking memory"
		exit_code=1
	fi
done
exit $exit_code
