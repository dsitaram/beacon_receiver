#!/usr/bin/env python

import stuffr
import numpy as n

# 
launch_window = [19.0, 20.0]

def write_care(f,year=2015,month=9,day=5,i0=0,ddir="."):
#    dt=60.0*60.0
    nt = len(launch_window)#24.0*3600.0/dt
    satf = file("%s/sats.txt"%(ddir),"w")
    t0 = stuffr.date2unix(year, month, day, 0, 0, 0)
    for ti in range(int(nt)):
        lt0 = launch_window[ti]*3600.0+t0
        lt1 = launch_window[ti]*3600.0+3600.0+t0
        sstr = "%03d CARE II %s %s"%(ti+i0,stuffr.unix2datestr(lt0),stuffr.unix2datestr(lt1))
        print(sstr)
        satf.write("%s\n"%(sstr))
        f.write("%1.5f %1.5f 150.012000 400.032000 40.000 40.000 75.272 CAREII\n"%(lt0,lt1))
    satf.close()

if __name__ == "__main__":
    f=file("care.txt","w")
    write_care(f,year=2015,month=9,day=5)
    f.close()
