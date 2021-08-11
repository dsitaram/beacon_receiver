#!/usr/bin/env python

import numpy as n
import stuffr

import optparse

if __name__ == "__main__":
    parser = optparse.OptionParser()

    parser.add_option('-f', '--file', dest='file', type="string", default="passes.txt")
    (op, args) = parser.parse_args()

    f=file(op.file,"r")
    f.readline()
    t0=[]
    t1=[]
    freq0=[]
    freq1=[]    
    name=[]            
    for l in f:
        ll=l.split(" ")
        t0.append(float(ll[0]))
        t1.append(float(ll[1])  )
        freq0.append(float(ll[2]))
        freq1.append(float(ll[3]))
        name.append(ll[7].strip())  
    f.close()
    t0=n.array(t0)
#    print(t0)
    t1=n.array(t1)    
    freq0=n.array(freq0)
    freq1=n.array(freq1)    
    idx=n.argsort(t0)
    for i in idx:
        print("%s-%s UTC %s %1.2f MHz %1.2f MHz"%(stuffr.unix2datestr(t0[i]),stuffr.unix2datestr(t1[i]),name[i],freq0[i],freq1[i]))
