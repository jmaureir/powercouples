#!/bin/bash
#SBATCH -J pc-mp
#SBATCH -o pc-mp.%j.out
#SBATCH -e pc-mp.%j.err

module load python/2.7.10

python powercouples-mp.py -i chunk.gnu.*
