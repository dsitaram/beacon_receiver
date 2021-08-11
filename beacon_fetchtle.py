#!/usr/bin/python3
"""
   Read beacon satellite TLEs from NORAD and search for satellites.
   output a beacon.tle format. 

   Usage: ./get_beacontle.py /data/beacon/data/2011.06.28/

   (c) 2010 Juha Vierinen
"""
import re
import urllib.request
import os, errno, sys
import pdb

#proxies = {'http': 'http://wwwproxy.fmi.fi:8080'}
proxies = None

def mkdir_p(path):
    try:
        os.makedirs(path)
    except OSError as exc: # Python >2.5
        if exc.errno == errno.EEXIST:
            pass
        else: raise

def goto_dir(startd):
    # the start date in pyephem format: 
    dtuple = startd.tuple()
    dirname = "%04d.%02d.%02d" % (dtuple[0],dtuple[1],dtuple[2])
    mkdir_p(dirname)
    os.chdir(dirname)

# list of beacon satellites and their frequencies.
class satellitelist:
    def __init__(self, fname):
        self.satnames = []
        self.offsets = []
        
        f = open(fname, 'r')
        data = f.readlines()
        f.close()
        
        for i in range(0,len(data)):
            (sn,of) = data[i].split(",")
            
            self.satnames.append(sn.strip())
            self.offsets.append(float(of))


def writeBeaconsTLE(fname,sl):
    os.makedirs(os.path.dirname(fname), exist_ok='True')
    f = open(fname, 'w')
    #
    # Read all relevant files fron celestrak
    #
    a = None
    try:

        #all active satellites
        with urllib.request.urlopen('https://celestrak.com/NORAD/elements/active.txt') as url:
            a = url.readlines()

    except IOError:
        print("Could not retrieve ephemeris from celestrak, reverting to cached file. Check internet connection!")
        f.close()
        os.system("cp beacon.tle.cache %s"%(fname))
        return(False)

    
    print("Read %f satellites" % (len(a)/3)) 
    for i in range(0, int(len(a)/3)):
        name = a[i*3].strip()
	
        for j in range(0,int(len(sl.satnames))):
        
            name = str(name)
            
            if sl.satnames[j] in name:
                uname = re.sub(" ","_",sl.satnames[j])
                print("HAHA")
         
                print("Found %s" % sl.satnames[j])
                f.write("%s\n" % (uname))
                
                line2 = re.sub("b'","",str(a[i*3+1].strip()))
                line2 = re.sub("'","",line2)
                
                line3 = re.sub("b'","",str(a[i*3+2].strip()))
                line3 = re.sub("'","",line3)
                
                
                f.write("%s\n" % (line2))
                f.write("%s\n" % (line3))
                f.write("%d\n" % (int(sl.offsets[j])))
    f.close()
    os.system("cp %s beacon.tle.cache"%(fname))
    return(True)

def download_tle_files(download_dir):
    sl = satellitelist("satellites.txt")
    return(writeBeaconsTLE("%s/beacon.tle"%(download_dir),sl))

if __name__ == "__main__":                
    sl = satellitelist("satellites.txt")
    ddir = sys.argv[1]

    writeBeaconsTLE("%s/beacon.tle"%(ddir),sl)
