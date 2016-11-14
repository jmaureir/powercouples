import os
import sys
import argparse
import csv
import itertools
import pew.pew as pw
import threading

def pow(x):
    return x[0]**x[1]

def pewT(x):
    return pw.pew(x[0],x[1])

def find_powerCouple(numbers):
    tuples = itertools.permutations(numbers,2)
    return max(tuples, key=pewT)

def worker(infile,out,lock):
    for row in csv.reader(infile):
        name = row[0]
        numbers = [int(i) for i in row[1:] ]
        pc = find_powerCouple(numbers)
        with lock:
            out.writerow( (name, pc[0], pc[1]) )
   
    return True

if __name__ == "__main__":  
    parser = argparse.ArgumentParser(description='PowerCouples Serial native version') 
    parser.add_argument('-i','--inputs',nargs='+', 
        dest="inputs_csv", help="list of input file in csv format", 
        required=True, type=argparse.FileType('r'))

    parser.add_argument('-o','--output', dest="output_csv", 
        help="output file in csv format", default=sys.stdout, 
        type=argparse.FileType('w'))

    args = parser.parse_args()
    out = csv.writer(args.output_csv)

    out_lck = threading.Lock()
    threads = []
    for infile in args.inputs_csv:
        t = threading.Thread(target=worker,args=(infile,out,out_lck))
        threads.append(t)
        t.start()

    print "waiting for termination"
    for t in threads:
        t.join()

    print "done"
