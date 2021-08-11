#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Wed Aug 11 10:41:17 2021

@author: sitardp1

implementation of flights.cc and flights.h in python
"""

import datetime
import time
from dataclasses import dataclass

@dataclass
class Satellite:
    startTime: datetime
    endTime: datetime
    freq: float
    usrpChannel: str
    bw: float
    phaseInc: float #supposed to be complex
    cordOsc: float #supposed to be complex
    pkE: float #peak elevation
    name: str #satellite name
    fname: str #name
    re_acc: float #real accumulator
    im_acc: float #imaginary accumulators
    acc_ind: int #accumulator index

@dataclass    
class Flight:
    satellite: Satellite
    day: datetime
    active: int # which satellites are active
    active_idx: int #which satellites are active
    nsatellites: int
    nactive: int
    dataDir: str
    logFile: file
    
    
def complex_mul(complex_float, complex_float, a , res):

#checks if the datetime is equal to the class time    
def is_dateTime_day_equal(dateTime):
        if (dateTime == d):
        return True
    else: 
        return False

