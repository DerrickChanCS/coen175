make clean
make
./scc < test.c 2> out.txt
cat out.txt
