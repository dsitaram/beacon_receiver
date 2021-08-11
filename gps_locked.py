#!/usr/bin/python3
##################################################
# Gnuradio Python Flow Graph
# Title: Top Block
# Generated: Mon Oct 24 13:02:28 2011
##################################################


from gnuradio import gr
from gnuradio import uhd


u = uhd.usrp_source(device_addr="addr=192.168.10.13",
                    io_type=uhd.io_type.COMPLEX_FLOAT32,
                    num_channels=1,
                    )
print(u.get_mboard_sensor("gps_time"))
print(u.get_mboard_sensor("gps_locked", 0))
print(u.get_mboard_sensor("gps_gprmc", 0))

