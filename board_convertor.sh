#!/bin/bash

# Converting board to inpu string

if [ $# -eq 0 ]; then
    echo
    echo "Please provide benchmark file."
    echo "e.g.  ./board_convertor.sh ./puzzles_benchmark.raw"
    echo "      The result will be output to file: puzzles_benchmark.txt"
    echo
    exit 1
fi

outfile=`echo $1 | sed -e "s/raw/txt/g"`
echo $outfile

rm -f $outfile
touch $outfile

oneline=""
while read LINE
do
    if [[ ${LINE:0:1} != [A-Z] ]];
    then
        LINE=`echo $LINE | sed -e 's/[\r,]//g'`
        oneline=${oneline}${LINE}
    else
        [ ! -z "$oneline" ] && echo $oneline >> $outfile
        oneline=""
    fi
done < $1
        
[ ! -z "$oneline" ] && echo $oneline >> $outfile
