make clean
make

arrc=(./examples/*.c)
arrout=(./examples/*.err)

for ((i=0; i<${#arrc[@]}; i++)); do
    echo "TESTING: ${arrc[$i]}"
    ./scc < ${arrc[$i]} > cout 2> temp.txt  && diff ${arrout[$i]} temp.txt
done

tar cvf ../phase2.tar .
