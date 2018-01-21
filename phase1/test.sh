lex -t lexer.l > lexer.cpp
g++ -o scc lexer.cpp

./scc < test.c > test.txt && cat test.txt
