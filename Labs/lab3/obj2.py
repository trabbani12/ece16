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
while(int(word[0])<1000):
    s = ser.readline()
    words = s.split()

    word[0]=words[0]
    a = float(words[1])
    a = a/1000000.0
    b = float(words[2])

#        word[3]=datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    data1.append(data1[counter] + a)
    data2.append(b)
    temp=(b/1024)
    temp=float(3.3*temp)
    temp=temp*3600.0 + 1.5
    datascaled.append(temp)
    counter=counter+1





x=np.array(data1)
y=np.array(data2)
#plt.plot(x,y)
#plt.show()
y2=np.array(datascaled)
plt.subplot(2,1,1)
plt.plot(x,y2)

b_high, a_high = scipy.signal.butter(3, 0.1, 'high', analog=False)
signal_out = scipy.signal.lfilter(b_high, a_high, datascaled)

b_low, a_low = scipy.signal.butter(3, .5, 'low', analog=False)
signal_out = scipy.signal.lfilter(b_low, a_low, datascaled)



box = scipy.signal.boxcar(100)
signal_out = scipy.signal.lfilter(box, 1, datascaled)

y3=np.array(abs(signal_out))
plt.subplot(2,1,2)
plt.plot(x,y3)
plt.show()




ser.close()
