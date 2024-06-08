#!/usr/bin/env python3
# -*- coding: utf-8 -*-
"""
Created on Thu May  2 20:32:21 2024

@author: adityachezhiyan
"""

import serial
from datetime import datetime
from scipy.stats import expon
import numpy as np

#Port used for the Arduino (check with Arduino IDE/Command Line)
port = "/dev/cu.usbmodem101"
#Notes added to the file name, change as needed
notes = "mightyohm_Co_60_lead_3"
file_name = ("count_times_%s_%s.txt" % (str(datetime.now()).replace(":", ""), notes))

#Returns counts rate (per second) and its error
#This requires that the environment does not change over the measurement period
def rate(ds):
    r = 1 / expon.fit(ds, floc=0, scale=np.average(ds))[1]
    return r, r / (len(ds) ** 0.5)

#Measurement period in seconds
period = 180

deltas = []
total = 0
ser = serial.Serial(port, 9600)
while True:
    time = int.from_bytes(ser.read(size=4), "little") / 1000000.0
    if time > 0.0015 and time < period:
        deltas.append(time)
        print(time)
        total += time
    if total > period:
        break
    ser.flush()

ser.flush()
ser.close()
with open(file_name, "w") as output_file:
    r = rate(deltas)
    print(r)
    output_file.write(("Rate (cps): %s" % r[0]) + "\n")
    output_file.write(("Error: %s" % r[1]) + "\n")
    for time in deltas:
        output_file.write(str(time) + "\n")