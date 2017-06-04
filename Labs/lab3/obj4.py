import os
import serial   #import serial library
import re #used for spliting white spaces
import datetime
import numpy as np
import matplotlib.pyplot as plt
import scipy.signal

ser = serial.Serial("/dev/cu.usbmodem1421", 115200) # create serial object
                                            # pointing to arduino connection

decision = raw_input("Enter Any Character to start reading from serial port\n")
ser.readline()


ser.write(decision)
ser.readline()
word = [0,0,0,0]

data1=[0]
data2=[0]
datascaled=[0]

counter=0;
while(int(word[0])<5000):
    s = ser.readline()
    words = s.split()

    word[0]=words[0]
    a = float(words[1])
    a = a/1000000.0
    b = float(words[2])
    data1.append(data1[counter] + a)
    data2.append(b)
    temp=(b/1024)
    temp=float(3.3*temp)
    temp=(temp - 1.5)/3600.0
    datascaled.append(temp)
    counter=counter+1





x=np.array(data1)
y=np.array(data2)

#high pass filter
b_high, a_high = scipy.signal.butter(3, 0.1, 'high', analog=False)
signal_out = scipy.signal.lfilter(b_high, a_high, datascaled)
#low pass filter
b_low, a_low = scipy.signal.butter(3, .5, 'low', analog=False)
signal_out1 = scipy.signal.lfilter(b_low, a_low, signal_out)


#box filter
box = scipy.signal.boxcar(100)
signal_out2 = scipy.signal.lfilter(box, 1, signal_out1)
#rectification done
y3=np.array(signal_out2)
datatime=[0]
dataprocessed=[0]
arr1=[0]
arr2=[0]


plt.ion()
#counter1=0
for counter1 in range(200):
    if (counter1>19):
        #datatime.extend(data1[(counter1-19)*20:(counter1)*20])
        #dataprocessed.extend(signal_out[(counter1-19)*20:(counter1)*20])
        arr1[0:399]=data1[(counter1-19)*20:(counter1)*20]
        arr2[0:399]=signal_out2[(counter1-19)*20:(counter1)*20]
    else:
#        datatime.extend(data1[0:(counter1+1)*20])
#        dataprocessed.extend(signal_out[0:(counter1+1)*20])
        arr1[0:399]=data1[0:(counter1+1)*20]
        arr2[0:399]=signal_out2[0:(counter1+1)*20]
    z1=np.array(arr1)
    z2=np.array(arr2)
    plt.plot(z1,z2)
    plt.pause(0.05)
    plt.hold(False)
#    counter1=counter1+1
while True:
    plt.pause(0.05)







ser.close()
