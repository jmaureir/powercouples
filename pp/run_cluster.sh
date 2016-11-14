#!/bin/bash
#
# PowerCouples Parallel Python version
#
# starting script
# Juan Carlos Maureira

IN_FILES=($@)
NUM_FILES=${#IN_FILES[@]}
CORES=20
NUM_WORKERS=`echo "scale=1; ($NUM_FILES / $CORES) + 0.5" | bc | cut -f 1 -d"."`
PORT=5000
SECRET="my_secret"

module load python/2.7.10

function deploy_workers() {
    let NODES=$1

    RESOURCES=""

    if [ $NODES -le 1 ]; then
        CORES=$NUM_FILES
        RESOURCES="-n1 -c $CORES"
    else 
        RESOURCES="-N $NODES -c $CORES"
    fi

    echo "running for $1 workers"

    srun --exclusive $RESOURCES -J ppserver ~/.local/bin/ppserver.py -w $CORES -a -p $PORT -s $SECRET

    echo "closing workers..."
}

if [ $NUM_WORKERS -eq 0 ]; then
    echo "No input files given"
    exit 1
fi

deploy_workers $NUM_WORKERS &

sleep 1
python ./powercouples-pp.py -i ${IN_FILES[@]}
sleep 1

scancel --name ppserver -s INT

wait 

echo "done"

