lex -t lexer.l > lexer.cpp
g++ -o scc lexer.cpp

./scc < fib.c > test.txt && diff fib.out test.txt
./scc < hello.c > test.txt && diff hello.out test.txt
./scc < list.c > test.txt && diff list.out test.txt
./scc < malloc.c > test.txt && diff malloc.out test.txt
./scc < sum.c > test.txt && diff sum.out test.txt
./scc < tricky.c > test.txt && diff tricky.out test.txt

tar cvf ../phase1.tar .
