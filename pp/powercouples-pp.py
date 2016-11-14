import os
import sys
import argparse
import csv
import itertools
import pew.pew as pw
import pp

def pow(x):
    return x[0]**x[1]

def pewT(x):
    return pw.pew(x[0],x[1])

def find_powerCouple(numbers):
    tuples = itertools.permutations(numbers,2)
    return max(tuples, key=pewT)

def worker(infile):
    results = []
    for row in csv.reader(infile):
        name = row[0]
        numbers = [int(i) for i in row[1:] ]
        pc = find_powerCouple(numbers)
        results.append( (name, pc[0], pc[1]) )

    return results

if __name__ == "__main__":    
    parser = argparse.ArgumentParser(description='PowerCouples Parallel Python (pp) version') 
    parser.add_argument('-i','--inputs',nargs='+', dest="inputs_csv", 
        help="list of input file in csv format", required=True, 
        type=argparse.FileType('r'))
    parser.add_argument('-o','--output', dest="output_csv", 
        help="output file in csv format", default=sys.stdout, 
        type=argparse.FileType('w'))

    args = parser.parse_args()
    out = csv.writer(args.output_csv)
    ncpus = len(args.inputs_csv)
    jobs = []
    #ppservers = ();
    ppservers = ("*:5000",)
    #job_server = pp.Server(ncpus, ppservers=ppservers)
    job_server = pp.Server(ncpus=0,ppservers=ppservers,secret="my_secret")

    for infile in args.inputs_csv:
        f = list(infile,)
        jobs.append(job_server.submit(worker,(f,), (find_powerCouple,pewT,pow), ("csv","itertools","pew.pew as pw"))) 
    
    for job in jobs: 
        out.writerows(job())
         
    job_server.print_stats()

