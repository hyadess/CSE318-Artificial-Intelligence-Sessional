g++ -c -O9 1905064.cpp -o e.o
g++ -O9 e.o -o e.exe
rm -r e.o

for(( i=0; i<20;i++))
do
./e.exe 80
done