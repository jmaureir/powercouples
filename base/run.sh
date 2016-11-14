#!/bin/bash

module load python/2.7.10

INFILES=`ls ../data/input_*x500.csv`

TIMEFORMAT=%R

for file in $INFILES;
do
    resfile="$file.result"
    output="$file.output"
    echo "running $resfile"
    { time python powercouples.py -i $file -o $resfile ; } 2> $output
    out=`cat $output`
    f=`basename $file`
    echo $f $out
    rm $output
done
