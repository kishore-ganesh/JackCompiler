# JackCompiler
A compiler for the Jack Programming Language, as specified in Nand2Tetris

This was built in December 2019 as part of the Nand2Tetris software toolchain.

The compiler is built as per specifications in the Nand2Tetris book, but the code is my own. It compiles down to a Virtual Machine language, which can further be translated using a VM Translator (http://github.com/kishore-ganesh/VMTranslator) and this Virtual Machine code can be translated into bare binary for the Hack computer using an Assembler (https://github.com/kishore-ganesh/JackAssembler).

There are two versions of the Compiler: 
1. One that generates XML code (This is to show that the Compiler understands the underlying code structure)
2. One that generates VM code (This is the finished version of the compiler)
