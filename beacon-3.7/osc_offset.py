#!/usr/bin/env python

import h5py
import numpy
import scipy
import stuffr
import scipy.constants
import scipy.interpolate
import matplotlib.pyplot as plt
import csv
import os
import time

import beacon_phasecurve
import beacon_conf

from optparse import OptionParser

parser = OptionParser()
    
if __name__ == "__main__":
    parser.add_option("-d", "--data_dir", dest="data_dir", type="string", default=".",
                        help="Data directory. (default %default)")

    (op, args) = parser.parse_args()

    if op.data_dir == ".":
        tnow = time.time()
        t = time.gmtime(tnow-24.0*3600.0)
        prevday = "%04d.%02d.%02d"%(t.tm_year,t.tm_mon,t.tm_mday)
        print(prevday)
        op.data_dir = "%s/%s"%(beacon_conf.datadir,prevday)
        print(op.data_dir)

    ppm = beacon_phasecurve.estimate_offsets(op.data_dir)

    print("ppm %1.2f"%(ppm))
    h = h5py.File("%s/offset.h5"%(beacon_conf.datadir),"w")
    h["offset_ppm"]=ppm
    h.close()


