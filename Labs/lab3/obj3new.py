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

datatime=[0]
dataraw=[0]
datascaled=[0]
dataHP=[0]
dataLP=[0]
dataBP=[0]
dataREC=[0]
dataBOX=[0]

counter=0;

while(int(counter)<1000):
    s=ser.readline()
    f.write(s)
    counter=counter+1
f.close()

fs = open(text_name + ".txt","r")
counter=0


while(counter<1000):
    s = fs.readline()
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
    h=float(words[4])
    dataLP.append(h)
    e=float(words[5])
    dataBP.append(e)
    f=float(words[6])
    dataREC.append(f)
    g=float(words[7])
    dataBOX.append(g)
    counter=counter+1




anp=np.array(datatime)
bnp=np.array(dataraw)
cnp=np.array(datascaled)
dnp=np.array(dataHP)
enp=np.array(dataLP)
fnp=np.array(dataREC)
gnp=np.array(dataBOX)
hnp=np.array(dataBP)
#raw data
plt.subplot(7,1,1)
plt.plot(anp,bnp)
plt.title("Raw Data")
plt.ylabel("Millivolts")
#scaled data
plt.subplot(7,1,2)
plt.plot(anp,cnp)
plt.title("Scaled Data")
#after High pass
plt.subplot(7,1,3)
plt.plot(anp,dnp)
plt.title("High-Passed Data")
#after low pass
plt.subplot(7,1,4)
plt.plot(anp,enp)
plt.title("Low Passed Data")
#after Band pass
plt.subplot(7,1,5)
plt.plot(anp,hnp)
plt.title("Band Passed Data")
#after rectification
plt.subplot(7,1,6)
plt.plot(anp,fnp)
plt.title("Rectified Data")
#after BOX filter
plt.subplot(7,1,7)
plt.plot(anp,gnp)
plt.title("Smoothed Data")
plt.xlabel("Time in Seconds")
plt.subplots_adjust(hspace=0.7,wspace=0.35)


plt.show()




ser.close()
fs.close()
