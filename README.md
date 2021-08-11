# beacon_receiver


This is Juha Vierinen's "Jitter" beacon tomography receiver code, which is available here: https://www.sgo.fi/~j/jitter/web/

We found the following issues with using Jitter on current hardware, and are working on software changes to address these challenges:

The only Ettus SDR daughterboard capable of 150/400 MHz is the TVRX2, which is no longer available due to a supply chain issue. 
The WBX can do both those frequencies, but not at the same time. 
The BasicRx is advertised as 1-250MHz but does not include a downconverter, so it may have aliasing issues above 50MHz: 
    https://forums.ni.com/t5/USRP-Software-Radio/USRP-property/td-p/3675612?profile.language=en

One solution to receive both channels on one radio would be 2x WBX (or WBX + BasicRx) on a X300/X310, but our X300 failed to achieve reference lock. 
Another solution is to use 2x N200/N210 (note the WBX takes up two daughterboard slots). However this requires a different data recorder implementation. 

The software change to support these (and other) architecture changes is the single-channel support in rx_beacon.cc. 
Rather than rewrite this recorder, we have incorporated the Digital_RF THOR python-based data recorder, which has sophisticated file-handling and many other advantages. 

By removing the "beacon" executable (based on the C code in the repository), we also lose the flights.cc functionality. This is also to be reimplemented in python. 

The expected outcome (not done yet) is a more flexible Jitter with support for multiple channels if desired. 
