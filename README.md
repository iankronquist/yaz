The Yaz Interpreter
===================

Yaz is a made up programming language to practice C and to better understand
how programming languages work.
The goal of this project is to write a simple interpreter.
It should correctly parse the language into abstract syntax trees and
execute the provided expressions.

The word "yaz" is Turkish. It means "summer", when I plan on working on this
project, as well as the imperitive form of "write!"

TODO:
----
* Add lots of comments
* Squash all the warnings from `parser.h`
* Finish writing tests for parser
* Provide a formal definition of yaz, with BNF notation and all

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

The eventual goal is to have a parser and maybe even a vm. In theory you'd want
to run it like:
```sh
$ ./yaz test.yaz
```
