make
g++ -o scc lexer.cpp lexmain.cpp

arrc=(./examples/*.c)
arrout=(./examples/*.out)

for ((i=0; i<${#arrc[@]}; i++)); do
    echo "TESTING: ${arrc[$i]}"
    ./scc < ${arrc[$i]} > temp.txt  && diff ${arrout[$i]} temp.txt
done
