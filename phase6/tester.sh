make clean
make all

arrc=(./examples/*.c)
arrin=(./examples/*.in)
arrout=(./examples/*.out)

for ((i=0; i<${#arrc[@]}; i++)); do
    echo "TESTING: ${arrc[$i]}"
    ./scc < ${arrc[$i]} > out.s
    gcc -m32 out.s
    ./a.out <${arrin[$i]} >temp.txt
    diff ${arrout[$i]} temp.txt
done

make clean
tar cvf ../phase6.tar ../phase6
