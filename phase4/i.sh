make
./scc < examples/$1.c >out.txt 2>examples/$1.err
cat out.txt
cat examples/$1.err
