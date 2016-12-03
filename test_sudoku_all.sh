#!/bin/bash

# Test all boards in file

if [ $# -eq 0 ]; then
    echo
    echo "Please provide parameter: [hard/easy]"
    echo "e.g.  ./test_sudoku.sh easy"
    echo "      It will run all boards in puzzles_easy.txt"
    echo
    exit 1
fi

infile="./puzzles_"$1".txt"
if [ ! -e $infile ]; then
    echo "Error parameter"
    exit 1
fi

i=1
while read line
do
    START=$(date +%s.%N)

    ./sudoku $line > /dev/null

    DIFF=$(echo "$(date +%s.%N) - $START" | bc);
    printf "Board #%d, run time: %.6f seconds\n" $i ${DIFF}
    ((i=i+1))
done < $infile

