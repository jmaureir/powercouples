#!/bin/bash
#SBATCH --ntasks=20
#SBATCH --J pc-base

do_bench() {
    TIMEFORMAT=%R
    file=$1
    resfile="$file.result"
    output="$file.output"
    { time python powercouples.py -i $file -o $resfile ; } 2> $output
    out=`cat $output`
    f=`basename $file`
    echo $f $out
    rm $output
}

module load python/2.7.10

if [ "$1" == "" ]; then
    INFILES=`ls ../data/input_*x500.csv`

    for file in $INFILES;
    do
        echo "running benchmark for $file"
        srun --exclusive -n1 $0 $file &
    done

    echo "waiting for tasks to complete"

    wait
    echo "done"
else
    do_bench $1
fi
