#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Aug 11 10:41:17 2021

@author: sitardp1

implementation of flights.cc and flights.h in python
"""

import datetime as dt
import time
import calendar
from dataclasses import dataclass
import cmath



dataDir = "/data/beacon/data"
date = dt.datetime(2021, 8, 11)
dateString = date.strftime( "%Y.%m.%d")

#idk if we need all of these variables, but these are the ones in flights.h
satKeys = ["startTime", "endTime", "freq", "usrpChannel", "bw", "dec", "phaseInc", "cordOsc", "pkE", "name",
              "fname", "re_acc", "im_acc", "acc_ind"]
flightKeys = ["satellite", "day", "active", "active_idx", "nsatellites", "nactive", "dataDir", "logFile"]


flightDict = dict.fromkeys(flightKeys, None)
satDict = dict.fromkeys(satKeys, None)

#ex below of storing complex numbers
c = 1 + 2j
real = c.real
im = c.imag

with open(dataDir+dateString+"/passes.txt") as f:
    contents = f.readlines()
    
#then parse through passes.txt and assign variables
#create flights.log files - has the following variables. need to get rid of one set to adapt for only one frequency
"""
"name":row[12],
                        "idx":int(row[0]),
                        "t0":float(row[1]),
                        "t1":float(row[2]),
                        "fname0":row[3],
                        "fname1":row[4],
                        "freq0":float(row[5]),
                        "freq1":float(row[6]),
                        "dec0":int(row[7]),
                        "dec1":int(row[8]),
                        "sr0":float(row[9]),
                        "sr1":float(row[10])})
"""

#set decimation
#set bandwidth
#reset accumulators
#set t0 and t1
#initialize filenames
#set up cordic coefficients
#set active flags

#don't need the classes
@dataclass
class Satellite: #make it into dictionary
    startTime: dt.datetime
    endTime: dt.datetime
    freq: float
    usrpChannel: str
    bw: float
    dec: int #decimation
    phaseInc: float #supposed to be complex
    cordOsc: float #supposed to be complex
    pkE: float #peak elevation
    name: str #satellite name
    fname: str #name
    re_acc: float #real accumulator
    im_acc: float #imaginary accumulators
    acc_ind: int #accumulator index

@dataclass    
class Flight: #make this into dictionary
    satellite: Satellite
    day: dt
    active: int # which satellites are active
    active_idx: int #which satellites are active
    nsatellites: int
    nactive: int
    dataDir: str
    logFile: str

def get_unix_time_now():
    
    now = dt.datetime.now()
    unix = calendar.timegm(now.timetuple())
    
    return unix

def reset_coordic(satellite, t, channel, decimation):
    #

    
    
    
    
    
    
    
