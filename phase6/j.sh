make clean
make all
./scc <test1.c 2>err.txt >out.s
cat out.s
gcc -g -m32 out.s 
./a.out  

