make clean
make
./scc < test.c >cout.txt 2> cerr.txt
cat cerr.txt
