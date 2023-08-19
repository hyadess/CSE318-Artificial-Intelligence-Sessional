#!/bin/bash

g++ -c  offline_3_maxCut.cpp -o e.o
g++ e.o -o e.exe
rm -r e.o

for ((i = 41; i <= 54; i++)); do
    input="inputs/g$i.rud"
    echo "$input"
    ./e.exe "$input"
done 

shutdown -h now