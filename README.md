Instant Grader CLI
========================

[![Build Status](https://travis-ci.org/neungkl/instant-grader.svg?branch=master)](https://travis-ci.org/neungkl/instant-grader)

A tools for grading your code and test subject which is on the single file.

<img src="preview.png" width="600">

Video demo here : [Youtube](https://www.youtube.com/watch?v=9b4syW3GWWk)

No need to create a new file for test subject just put in the same file where you code.
The instant grader automatically split your code file to an individual test case.
Compile the file and run every test case. Then show the result to you.

For example. You write the add program in C language.
You want to test it with some test case.<br>
Here what its code look like.

```c
#include <stdio.h>
int main() {
  int a,b; scanf("%d%d",&a,&b);
  printf("%d\n", a + b);
}

/*
===
1 2
---
3
===
4 5
---
9
===
*/
```

## Installation

For MacOS user:

```sh
brew tap neungkl/instant-grader
brew install instant-grader
```

For Windows and Linux user:

1. Clone this project.
2. Build file `make build`
4. Paste `grader` to the same directory of you code. (Anywhere you want)
5. Run instant grader.

```sh
$ ./build/instant-grader YOUR_FILE
```

6. Done.

## Advance Usage

From the name of project. It's called CLI.
you can customize mostly thing you want in this tools.

### Command

```sh
$ instant-grader [OPTIONS] YOUR_CODE
$ instant-grader YOUR_CODE [OPTIONS]

# show help documentation
$ instant-grader -h
```

### Options

You can add some parameters to grader.<br>
Something like `instant-grader -v`
for expose all accepted subject and rejected subject.

| Option        | Description   | Default |
| ------------- |---------------| ------- |
| -v | Show content in both accepted and rejected case | - |
| -o | Specific execute file path | - |
| -b | Specific bin path | ./bin |
| -in STR | Input delimiter in script file | "===" |
| -out STR | Output delimiter in script file | "---" |
| --compile CMD | Custom compile command. | "gcc -o %prog %file" |
| --run CMD | Custom run command. | "%prog < %in > %out" |

Note: `-in` and `-out` delimiter matching only the head of content line.

### Options Pattern

Some options are freely used for "Non-specific file"

When you give the options `--compile` in command.<br>
You have to enter the `CMD` maybe look like `"gcc -o %prog %file"`

You don't know the "Exactly" program name.
(Because the instant grader will automatic generate program name)<br>
Can use `%prog` or `%file` instead.

| Option pattern | Description |
| -------------- | ----------- |
| %bin | Bin directory |
| %code | Your code file |
| %prog | Compiled program (for interpreted language this file not created) |
| %progx | Compiled program name (only a name not include directory path) |
| %in | Input test case file |
| %out | Output file after run case |

Note: `%in` and `%out` option is individual file for each test case.<br>
(Not all test case. The program will split test cases to individual file
  and put each test case to your program)

#### Option Pattern Scope

Some pattern not allow in some option.<br>
When you customize `--compile CMD`. You can't use `%in` `%out` pattern.

|            | %bin | %code | %prog | %progx | %in | %out |
| -----------|:----:|:-----:|:-----:|:------:|:---:|:----:|
| -o         |  Y   |       |       |   Y    |     |      |
| --compile  |  Y   |   Y   |   Y   |        |     |      |
| --run      |  Y   |   Y   |   Y   |   Y    |  Y  |   Y  |
Y Can use

Example:
- `--compile "gcc -o %prog %file"` Correct.
- `--compile "gcc -o %in %out"` Incorrect.
- `--run "python %code < %in > %out"` Correct.
- `-o "%code"` Incorrect.
- `-o "%bin/%progx"` Correct.

## Example

Currently tools are support 6 languages. (C, C++, Java, Python, Ruby, Haskell)<br>
If you need more language you can customize via `--compile` and `--run` option.<br>
Even more, customize delimiter for test case in your file can edit via `-in` and `-out` <br>

There are some sample file in `sampleFile` directory provide some basic stuff.
And some sample test case in each file. Also include a file with stack overflow behavior.
You can test it and play along with instant grader to see what its result look like.

#### Here a list of useful command

- Multi-language support (depend on extension of file). No need to configure anything.

```sh
$ instant-grader code.c
$ instant-grader code.cpp
$ instant-grader code.hs
$ instant-grader code.java
$ instant-grader code.py
$ instant-grader code.rb
```

- Verbose mode (Completely show all content in each test case).

```sh
$ instant-grader code.cpp -v
```

- Customize gcc compile with optimization level 3.

```sh
$ instant-grader code.c --compile "gcc -O3 -Wall -o %prog %code"
```

- Export compiled program.

```sh
$ instant-grader code.c -o code
$ instant-grader code.c -o code --compile "gcc -O3 -Wall -o %prog %code"
```

- Customize bin path

```sh
$ instant-grader code.cpp -b tmp/
```

- Customize delimieter

```cpp
#include <cstdio>
int main() {
  int N;
  scanf("%d", &N);
  printf("%d", N * 2);
}

/*
###
5
+++
10
###abc
7
+++eiei
14
###
*/
```

```sh
$ instant-grader code.cpp -in "###" -out "+++"
$ instant-grader code.cpp -in "##" -out "++"
$ instant-grader code.cpp -in "#" -out "+"
```

- Grade with Python file<br>
(Program currently support Python.
  No need to specific compile and run option.
  This is just example)

```sh
$ instant-grader code.py --compile "" --run "python %code < %in > %out"
```

- Grade with Java file

```sh
$ instant-grader code.java --compile "javac -d %bin %code" --run "java -classpath %bin %progx < %in > %out"
```

- Multi combination

```sh
$ instant-grader code.cpp -v -b tmp/ -o mycode --compile "g++ -Wall -O3 -o %prog %code" --run "%prog < %in > %out"
```

## Build

For own customization. Here how to build the instant grader

```sh
$ make build
```

While developing.
```sh
$ make
```

For testing
```sh
$ make test
```

## Limitation

### Platform support

Instant grader is supported in Linux and MacOS platform. Maybe Windows in future.<br>
But currently, Windows is not supported.

### Language support & Prerequisites

You NEED to install compiler of each language before using instant grader.
But NO setting for compile and run message.

- C (require gcc)
- C++ (require g++)
- Java (require javac, java)
- Python (require python)
- Ruby (require ruby)
- Haskell (require ghc)

If you using another language you can specific by `--compile` and `--run` option.
Remember to install necessary compiler before runs instant grader.

## Author

- Kosate Limpongsa (Project owner)

## License

[MIT](LICENSE) © Kosate Limpongsa
