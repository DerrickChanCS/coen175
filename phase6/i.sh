make clean
make all
./scc <examples/calc.c 2>err.txt >out.s
cat out.s
gcc -g -m32 out.s 
./a.out  <examples/calc.in >out.txt
diff out.txt examples/calc.out
