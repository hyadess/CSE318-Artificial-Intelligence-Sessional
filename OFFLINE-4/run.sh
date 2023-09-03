g++ -c -O9 1905064.cpp -o e.o
g++ -O9 e.o -o e.exe
rm -r e.o
rm -f "output.csv"
rm -f "eval.txt"
for((itr=10;itr<=99;itr++))
do

for(( i=0; i<20;i++))
do
./e.exe $itr
done


count=0
sum=0
sum_squares=0

while IFS= read -r line; do
  
    if [[ "$line" =~ ^[0-9]+(\.[0-9]+)?$ ]]; then
        number=$(echo "$line" | sed 's/,/./') # Handle decimal point as either '.' or ','
        sum=$(awk "BEGIN {print $sum + $number}")
        sum_squares=$(awk "BEGIN {print $sum_squares + ($number * $number)}")
        ((count++))
    fi
done < "eval.txt"

rm -f "eval.txt"

mean=$(awk "BEGIN {print $sum / $count}")

variance=$(awk "BEGIN {print ($sum_squares / $count) - ($mean * $mean)}")

standard_deviation=$(awk "BEGIN {print sqrt($variance)}")


echo "$itr,$mean,$standard_deviation" >> "output.csv"

done

