The Yaz Interpreter
===================

[![Build Status](https://travis-ci.org/iankronquist/yaz.svg?branch=master)](https://travis-ci.org/iankronquist/yaz.svg?branch=master)

Yaz is a made up programming language to practice C and to better understand
how programming languages work.
The goal of this project is to write a simple interpreter.
It should correctly parse the language into abstract syntax trees and
execute the provided expressions.

The word "yaz" is Turkish. It means "summer", when I plan on working on this
project, as well as the imperitive form of "write!"

TODO:
----
* Finish writing tests for parser
* Provide a formal definition of yaz, with BNF notation and all

Hacking
------
It's pretty straightforwards to play with.
To make (currently broken):
```sh
$ make
```

To test (requires valgrind):
```sh
$ make tests
$ ./runtests.sh
```

The eventual goal is to have a parser and maybe even a vm. In theory you'd want
to run it like:
```sh
$ ./yaz test.yaz
```

Documentation
-------------
Docs are generated using Doxygen. Install Doxygen and run:

```sh
$ make docs
```

By default html docs will be built. A Makefile to generate Latex docs can be
found in `docs/latex`.
