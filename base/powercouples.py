import os
import sys
import argparse
import csv
import itertools
import pew.pew as p

def pow(x):
    return x[0]**x[1]

def pewT(x):
    return p.pew(x[0],x[1])

def find_powerCouple(numbers):
    tuples = itertools.permutations(numbers,2)
    return max(tuples, key=pewT)

if __name__ == "__main__":    
    parser = argparse.ArgumentParser(description='PowerCouples Serial native version') 
    parser.add_argument('-i','--input', dest="input_csv", help="input file in csv format", required=True, type=argparse.FileType('r'))
    parser.add_argument('-o','--output', dest="output_csv", help="output file in csv format", default=sys.stdout, type=argparse.FileType('w'))

    args = parser.parse_args()

    out = csv.writer(args.output_csv)

    for row in csv.reader(args.input_csv):
        name = row[0]
        numbers = [int(i) for i in row[1:] ]
        pc = find_powerCouple(numbers)
        out.writerow( (name, pc[0], pc[1]) )


