#!/bin/bash

g++ -c -O9 offline_3_maxCut.cpp -o e.o
g++ -O9 e.o -o e.exe
rm -r e.o

for ((i = 1; i <= 54; i++)); do
    input="inputs/g$i.rud"
    echo "$input"
    ./e.exe "$input"
done 

#shutdown -h now