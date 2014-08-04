The Yaz Interpreter
===================


Yaz is a made up programming language to practice C and to better understand
how programming languages work.
The goal of this project is to write a simple interpreter for a lisp-like
langauge. It should correctly parse the language into abstract syntax trees and
execute the provided expressions.

The word "yaz" is Turkish. It means "summer", when I plan on working on this
project, as well as the imperitive form of "write!"

TODO:
----
* Write more docs
* Write more tests
* Provide a formal definition of yaz, with BNF notation and all
* Actually parse the asts

Hacking
------
It's pretty straightforwards to play with, although it's not the most elegant.
To make:
```sh
$ make
```

To test (requires valgrind):
```sh
$ make tests
$ ./runtests
```

To make docs (requires doxygen):
```sh
$ make docs
```

Right now you can run parser out of `bin`. It will print out a representation
of the asts which should be pretty similar to the yaz source.
```sh
$ ./build/yazparser test.yaz
```

The eventual goal is to have a parser and maybe even a vm. In theory you'd want
to run it like:
```sh
$ ./yaz test.yaz
```
