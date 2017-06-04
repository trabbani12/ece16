import os
import serial   #import serial library
import re #used for spliting white spaces
import datetime
import numpy as np
import matplotlib.pyplot as plt
import scipy.signal

ser = serial.Serial("/dev/cu.usbmodem1421", 115200) # create serial object
                                            # pointing to arduino connection
text_name = raw_input("Enter the name of the text file you would like to output to: ")

path = os.path.realpath(__file__) #get the path of the currently executing python file
if text_name[-4:] == ".txt":
    text_name = text_name[:-4]
print text_name
if not os.path.exists(path[:-19]  + "/" + text_name + ".txt"):
    f = open(text_name + ".txt","w")
elif os.path.exists(path[:-19]  + "/" + text_name + ".txt") and not os.access(path[:-19]  + "/" + text_name + ".txt", os.W_OK):
    print "That file currently exists, but you do not have access to overwrite it. Creating " + text_name + "_two.txt"
    f = open(text_name + "_two.txt", "w")
elif os.path.exists(path[:-19]  + "/" + text_name + ".txt") and os.access(path[:-19]  + "/" + text_name + ".txt", os.W_OK):
    choice = raw_input("The file you have specified already exists and you have access to it. Would you like to\n1)Continue output to the same file\n2)Overwrite the file\n3)Create a second file " + text_name + "_two.txt\n")
    if (int(choice) == 1):
        f = open(text_name + ".txt","w")
    elif (int(choice) == 2):
        f = open(text_name + ".txt","a")
    elif (int(choice) == 3):
        f = open(text_name + "_two.txt", "w")
    else:
        print "You have entered an invalid input, goodbye."
        exit()


decision = raw_input("Enter Any Character to start reading from serial port\n")
ser.readline()


ser.write(decision)
ser.readline()
word = [0,0,0,0]

data1=[0]
data2=[0]
datascaled=[0]

counter=0;

while(int(counter)<5000):
    s=ser.readline()
    words = s.split()
    word[0]=words[0]
    a = float(words[1])
    a = a/1000000.0
    b = float(words[2])
    data1.append(data1[counter] + a)
    data2.append(b)
    temp=(b/1024)
    temp=float(3.3*temp)
    temp=temp*3600.0 + 1.5
    datascaled.append(temp)
    counter=counter+1


#high pass filter
b_high, a_high = scipy.signal.butter(3, 0.1, 'high', analog=False)
signal_out = scipy.signal.lfilter(b_high, a_high, datascaled)
#low pass filter
b_low, a_low = scipy.signal.butter(3, .5, 'low', analog=False)
signal_out1 = scipy.signal.lfilter(b_low, a_low, signal_out)

#box filter
box = scipy.signal.boxcar(100)
signal_out2 = scipy.signal.lfilter(box, 1, signal_out1)



toprint1=list(signal_out2)
counter1=0

while(int(counter1)<5000):
    f.write(str(data1[counter1+1]) + "\t" + str(toprint1[counter1+1]) + "\n")
    counter1=counter1+1

f.close()

fs = open(text_name + ".txt","r")


xar=[]
yar=[]
nums=[0,0,0]
c1=0
while (c1<200):
    plt.ion()
    for i in range(19):
        s=fs.readline()
        nums=s.split()
        xar.append(float(nums[0]))
        yar.append(float(nums[1]))
    plt.hold(False)
    plt.plot(xar, yar)








ser.close()
