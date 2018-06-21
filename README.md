# Kaleidoscope: Implementing a Language with LLVM

## How to build it

On Ubuntu (tested on 16.04).
```
Install llvm (tested 5.0, but it should work for other versions as well)
See http://apt.llvm.org/
cmake .
make
./main
This should bring up a simple repl
```

## Components

The code here contains the lexer, AST and parser of Kaleidoscope. The IR code generation and optimization are also available. In other words, it is the content of LLVM Tutorial Chapter 1-4.

No JIT, no Chapter 5 - 7 (If, Loop, ...).

Chapter 8 "Compile to Object code" somehow doesn't work for this CMake project. Directly call LLC instead.

## Generate IR in a terminal and file

Enter in the terminal

```
ready> def average(x y) (x + y) * 0.5;
```

See the following sentences in the terminal
```
ready> Read function definition:
define double @average(double %x, double %y) {
entry:
  %addtmp = fadd double %x, %y
  %multmp = fmul double %addtmp, 5.000000e-01
  ret double %multmp
}
```

Press CTRL+D to properly exit.

A file called `output.ir` will be generated in the same folder

## Generate Assembly code from IR

Enter in the terminal. I use LLVM-5.0 which is not the default LLVM on Ubuntu 16.04 so I have to specify the version of LLC (LLVM static compiler).

Otherwise, "llc" command should work. See [LLC docs](http://llvm.org/docs/CommandGuide/llc.html).

```
llc-5.0 -filetype=asm output.ir -o output.s
```

## Generate object code from IR (Chapter 8)

Enter in the terminal. I use LLVM-5.0 which is not the default LLVM on Ubuntu 16.04 so I have to specify the version of LLC (LLVM static compiler).

Otherwise, "llc" command should work. See [LLC docs](http://llvm.org/docs/CommandGuide/llc.html).

```
llc-5.0 -filetype=obj output.ir -o output.o
```

## Link object code to an executable file (Chapter 8)
There is a tester executable file `tester.cpp` in this project:

```
#include <iostream>

extern "C" {
    double average(double, double);
}

int main() {
    std::cout << "average of 3.0 and 4.0: " << average(3.0, 4.0) << std::endl;
}
```

Compile 


```
clang++ tester.cpp output.o -o tester
```

Run

```
./tester
average of 3.0 and 4.0: 3.5
```

It calls the function that we entered at the beginning

```
def average(x y) (x + y) * 0.5;
```


---

### This repo is forked from https://github.com/kornilova-l/llvm-kaleidoscope. The README below is the original post.

## Why?

Self-education...

I'm interested in LLVM and want to try simple things with it.
That's why I've started official LLVM tutorial - [Kaleidoscope](http://llvm.org/docs/tutorial).

## What's all about?

This tutorial runs through the implementation of a simple language, showing how fun and easy it can be.
This tutorial will get you up and started as well as help to build a framework you can extend to other languages.
The code in this tutorial can also be used as a playground to hack on other LLVM specific things.

The goal of this tutorial is to progressively unveil our language, describing how it is built up over time.
This will let us cover a fairly broad range of language design and LLVM-specific usage issues, showing and explaining the code for it all along the way, without overwhelming you with tons of details up front.

It is useful to point out ahead of time that this tutorial is really about teaching compiler techniques and LLVM specifically, not about teaching modern and sane software engineering principles.
In practice, this means that we’ll take a number of shortcuts to simplify the exposition.
For example, the code uses global variables all over the place, doesn’t use nice design patterns like visitors, etc... but it is very simple.
If you dig in and use the code as a basis for future projects, fixing these deficiencies shouldn’t be hard.

## How it works all together?

### Lexer

The first thing here is a lexer.
Lexer is responsible for getting a stream of chars and translating it into a groups of tokens.

> A lexer is a software program that performs lexical analysis. Lexical analysis is the process of separating a stream of characters into different words, which in computer science we call 'tokens'.

Tokens identifiers are stored under `lexer/token.h` file and lexer implementation under `lexer/lexer.cpp` file.

Tokens are just an `enum` structure, which consists of token identifier and a number assigned to this token.
This way, we can identify tokens through lexical analysis.

The actual reading of a stream is implemented in `lexer/lexer.cpp` file.
Function `gettok` reads characters one-by-one from `stdin` and groups them in tokens.
So, basically, `gettok` function reads characters and returns numbers (tokens).

Further, we can use these tokens in parser (semantic analysis).

### AST (Abstract Syntax Tree)

Though, before diving into the parser, we need to implement AST nodes, that we can use during parsing.

Basic block of each AST node is `ExprAST` node, which is stored under `ast/ExprAST.h` file.
All other nodes are extends from `ExprAST` node.

Each of AST nodes must implement one method - `codegen()`.
`codegen()` method is responsible for generating LLVM IR, using LLVM IRBuilder API, that's all.

As you can see in `ast` folder, we have implemented the following AST nodes with appropriate code generation into LLVM IR:

- Binary Expressions;
- Call Expressions;
- Function Expressions;
- Number Expressions;
- Prototype Expressions;
- Variable Expressions;

Each of these nodes have a constructor where all mandatory values are initialized.
Based on that information, `codegen()` can build LLVM IR, usine these values.

The simplest one, i.e. is Number Expression.
`codegen()` for number expression just calls appropriate method in LLVM IR Builder:

```c++
llvm::Value *NumberExprAST::codegen() {
  return llvm::ConstantFP::get(TheContext, llvm::APFloat(Val));
}
```

Now, we have two parts of a compiler which we can combine.

### Parser

Parser is where lexer and AST are combined together.
The actual implementation of a parser stores into `parser/parser.cpp` file.

Parser uses lexer for getting a stream of tokens, which are used for building an AST, using our AST implementation.

So, in general, when parser sees a known token, i.e. number token, it tries to create a `NumberExprAST` node.

When parsing is done, got the last character/token from the stream, we have an AST representation of our code.
We can use it and generate LLVM IR from our AST using `codegen()` method in each AST node.

This process is done in `main.cpp` file.
`main.cpp` file is the place where all the parts are combined in one place.
