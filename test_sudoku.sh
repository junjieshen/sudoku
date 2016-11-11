#!/bin/bash

# Testing Sudoku

if [ $# -eq 0 ]; then
    echo
    echo "Please provide parameters: [hard/easy] [board number]"
    echo "e.g.  ./test_sudoku.sh easy 5"
    echo "      It will load the 5th line from puzzles_easy.txt"
    echo "      as the input parameter of sudoku. "
    echo
    exit 1
fi

if [ "$1" == "easy" ]; then
    infile="./puzzles_easy.txt"
else
    infile="./puzzles_hard.txt"
fi

if [ $2 -le 0 ]; then
    echo "Board number has to be greater than 0."
    exit 1
fi

line=`sed "$2!d" $infile`

if [ "$line" == "" ]; then
    echo "Board number out of range, use a smaller number."
    exit 1
fi

make clean >/dev/null
make >/dev/null

START=$(date +%s.%N)

./sudoku $line

DIFF=$(echo "$(date +%s.%N) - $START" | bc);
printf "Run time: %.6f seconds\n" ${DIFF}
