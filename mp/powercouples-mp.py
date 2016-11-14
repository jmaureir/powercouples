import os
import sys
import argparse
import csv
import itertools
import pew.pew as pw
import multiprocessing as mp


def pow(x):
    return x[0]**x[1]

def pewT(x):
    return pw.pew(x[0],x[1])

def find_powerCouple(numbers):
    tuples = itertools.permutations(numbers,2)
    return max(tuples, key=pewT)

def worker(infile,out_q):
    try:
        results = []
        print "processing %s",infile
        for row in csv.reader(infile):
            name = row[0]
            numbers = [int(i) for i in row[1:] ]
            pc = find_powerCouple(numbers)
            results.append( (name, pc[0], pc[1]) )
        out_q.put(results)
    except:
        print "worker failed" 
    finally:
        print "done"
    exit(0)

if __name__ == "__main__":    
    parser = argparse.ArgumentParser(description='PowerCouples Serial native version') 
    parser.add_argument('-i','--inputs',nargs='+', dest="inputs_csv", help="list of input file in csv format", required=True, type=argparse.FileType('r'))
    parser.add_argument('-o','--output', dest="output_csv", help="output file in csv format", default=sys.stdout, type=argparse.FileType('w'))

    args = parser.parse_args()
    out = csv.writer(args.output_csv)

    m = mp.Manager()
    result_queue = m.Queue()

    jobs = []
    for infile in args.inputs_csv:
        jobs.append( mp.Process(target=worker, args=(infile,result_queue)))
        jobs[-1].start()

    print "waiting for workers to finish"
    for p in jobs:
        p.join()
    
    print "writing results"

    num_res=result_queue.qsize()
    while num_res>0:
        out.writerows(result_queue.get())
        num_res -= 1

    
