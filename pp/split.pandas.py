#!/usr/bin/env python

import sys
import pandas as pd

bigfile=sys.argv[1]
lines=int(sys.argv[2])

for i,chunk in enumerate(pd.read_csv(bigfile, chunksize=lines)):
    chunk.to_csv('chunk{}.csv'.format(i),index=False)
    
