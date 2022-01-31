# MAVN-SPPuRV
MAVN compiler, translates higher level assembly Mips code to regular Mips32 assembly, for course SPPuRV, summer semester 2020/2021.
# How to use
After compiling, run program with two arguments, first is name of `.mavn` file which you want to translate, seconnd is name of `.s` file, which is target file
  ```
  g++ --std=c++20 FiniteStateMachine.cpp  Token.cpp LexicalAnalysis.cpp  IR.cpp SyntaxAnalysis.cpp main.cpp
  ./a.out test outTest
  ```
