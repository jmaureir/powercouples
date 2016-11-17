import os
import sys
import csv
import argparse
import random
import numpy.random as nprnd

from random import choice
from string import ascii_uppercase

def generate_rnd_string(s):
    return ''.join(choice(ascii_uppercase) for i in range(s))

if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='PowerCouples file generation')
    parser.add_argument('-r','--rows', dest="rows_num", help="rows number", required=True, type=int)
    parser.add_argument('-c','--cols', dest="cols_num", help="colums number", required=True, type=int)
    parser.add_argument('-m','--max', dest="max", help="rnd max", type=int, default=100)

    parser.add_argument('-o','--output', dest="output_csv", help="output file in csv format", default=sys.    stdout, type=argparse.FileType('w'))

    args = parser.parse_args()
    out = csv.writer(args.output_csv)

    for row in range(1,args.rows_num):
        name=generate_rnd_string(10)
        row=nprnd.randint(args.max, size=args.cols_num).tolist()
        row.insert(0,name)
        out.writerow( row )
