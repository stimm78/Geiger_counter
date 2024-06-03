#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu May  2 20:32:21 2024

@author: adityachezhiyan
"""

import serial
from datetime import datetime
import atexit

#Port used for the Arduino (check with Arduino IDE/Command Line)
port = "/dev/cu.usbmodem101"
#Notes added to the file name, change as needed
notes = "Sr90_10cm_425V"
file_name = ("count_times_%s_%s.txt" % (str(datetime.now()).replace(":", ""), notes))

#Measurement period in seconds
period = 180

def exit_handler():
    ser.flush()
    ser.close()
atexit.register(exit_handler)

with open(file_name, "w") as output_file:
    ser = serial.Serial(port, 9600)
    while True:
        time = int.from_bytes(ser.read(size=4), "little") / 1000000.0
        output_file.write(str(time) + "\n")
        print(time)
        if time > period:
            break
        ser.flush()