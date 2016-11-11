#!/bin/bash

# Converting board to inpu string

if [ $# -eq 0 ]; then
    echo
    echo "Please provide board file."
    echo "e.g.  ./board_convertor.sh ./puzzles_easy.raw"
    echo "      The result will be output to file: puzzles_easy.raw.out"
    echo
    exit 1
fi

outfile=$1.out
rm -f $outfile
touch $outfile

oneline=""
while read LINE
do
    if [ "$LINE" == "========" ]; then
        echo $oneline >> $outfile
        oneline=""
    else
        oneline=${oneline}${LINE}
    fi
done < $1
