import os
import sys
import argparse
import csv
import itertools
import pc.find as pc 

if __name__ == "__main__":    
    parser = argparse.ArgumentParser(description='PowerCouples Serial native version') 
    parser.add_argument('-i','--input', dest="input_csv", help="input file in csv format", required=True, type=argparse.FileType('r'))
    parser.add_argument('-o','--output', dest="output_csv", help="output file in csv format", default=sys.stdout, type=argparse.FileType('w'))

    args = parser.parse_args()

    out = csv.writer(args.output_csv)

    for row in csv.reader(args.input_csv):
        name = row[0]
        numbers = [int(i) for i in row[1:] ]
        t = pc.find_omp(numbers)
        out.writerow( (name, t[0], t[1]) )


