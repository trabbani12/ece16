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

datatime=[0]
dataraw=[0]
datascaled=[0]
dataHP=[0]
dataLP=[0]
dataREC=[0]
dataBOX=[0]

counter=0;
while(counter<1000):
    s = ser.readline()
    words = s.split()

    a = float(words[0])
    a = a/1000000.0
    datatime.append(a)
    b = float(words[1])
    dataraw.append(b)
    c=float(words[2])
    datascaled.append(c)
    d=float(words[3])
    dataHP.append(d)
    e=float(words[4])
    dataLP.append(e)
    f=float(words[5])
    dataREC.append(f)
    g=float(words[6])
    dataBOX.append(g)
    counter=counter+1




anp=np.array(datatime)
bnp=np.array(dataraw)
cnp=np.array(datascaled)
dnp=np.array(dataHP)
enp=np.array(dataLP)
fnp=np.array(dataREC)
gnp=np.array(dataBOX)
#raw data
plt.subplot(6,1,1)
plt.plot(anp,bnp)
#scaled dataBOX
plt.subplot(6,1,2)
plt.plot(anp,cnp)
#after High pass
plt.subplot(6,1,3)
plt.plot(anp,dnp)
#after low pass
plt.subplot(6,1,4)
plt.plot(anp,enp)
#after rectification
plt.subplot(6,1,5)
plt.plot(anp,fnp)
#after BOX filter
plt.subplot(6,1,6)
plt.plot(anp,gnp)



plt.show()




ser.close()
