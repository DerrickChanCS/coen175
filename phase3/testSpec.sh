make clean
make

fi=(./examples/${1}.c)
fo=(./examples/${1}.err)

./scc < $fi >cout.txt 2> cerr.txt && diff $fo cerr.txt
cat $fo
cat cerr.txt
