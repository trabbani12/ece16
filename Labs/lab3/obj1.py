import os
import serial   #import serial library
import re #used for spliting white spaces
import datetime
import numpy as np
import matplotlib.pyplot as plt

ser = serial.Serial("/dev/cu.usbmodem1421", 115200) # create serial object
                                            # pointing to arduino connection

decision = raw_input("Enter Any Character to start reading from serial port\n")
ser.readline()
#print ser.readline()

ser.write(decision)
ser.readline()
word = [0,0,0,0]

data1=[0]
data2=[0]
datascaled=[0]

counter=0;
while(int(word[0])<1000):
    s = ser.readline()
    #counter=counter++
    words = s.split()
    #print type(words)
    #print len(words)
    #print words
    #print s

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
plt.subplot(2,1,1)
plt.plot(x,y)
y2=np.array(datascaled)
plt.subplot(2,1,2)
plt.plot(x,y2)
plt.show()



ser.close()
