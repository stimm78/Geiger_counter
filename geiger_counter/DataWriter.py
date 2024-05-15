#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu May  2 20:32:21 2024

@author: adityachezhiyan
"""

import serial
from datetime import datetime
import atexit

port = "/dev/cu.usbmodem101"
notes = "rate_test"
file_name = "count_vs_time_%s_%s.txt" % (str(datetime.now()), notes)

def exit_handler():
    ser.flush()
    ser.close()
atexit.register(exit_handler)

with open(file_name, "w") as output_file:
    ser = serial.Serial(port, 9600)
    while True:
        time = int.from_bytes(ser.read_until(size=4), "little") / 1000000.0
        #time = ser.readline().decode("utf-8")
        output_file.write(str(time) + "\n")
        print(time)