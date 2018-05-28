make clean all

arrc=(./examples/*.c)
arrlib=(./examples/*.c)
arrout=(./examples/*.out)

for ((i=0; i<${#arrc[@]}; i++)); do
    echo "TESTING: ${arrc[$i]}"
    ./scc < ${arrc[$i]} > out.s 2> temp.txt
    gcc -m32 out.s 
done

