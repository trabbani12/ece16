import os
import serial   #import serial library
import re #used for spliting white spaces
import datetime
import numpy as np
import matplotlib.pyplot as plt
from matplotlib.ticker import NullFormatter

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
#print ser.readline()

ser.write(decision)
ser.readline()
word = [0,0,0,0]

data1=[0]
data2=[0]
datascaled=[0]

counter=0;

while(int(counter)<1000):
    s=ser.readline()
    f.write(s)
    counter=counter+1
f.close()

f = open(text_name + ".txt","r")
counter=0
while(int(word[0])<1000):
    s = f.readline()
    words = s.split()
    word[0]=words[0]
    a = float(words[1])
    a = a/1000000.0
    b = float(words[2])

    data1.append(data1[counter] + a)
    data2.append(b)
    temp=float(b/1024.0)
    temp=(3.3*temp)
    temp=(temp - 1.5)/3600.0
    datascaled.append(temp)
    counter=counter+1



x=np.array(data1)
y=np.array(data2)
plt.subplot(2,1,1)
plt.plot(x,y)
plt.xlabel('Time in Seconds')
plt.ylabel('Magnitude')
plt.title('Raw Data vs Time')
y2=np.array(datascaled)
plt.subplot(2,1,2)
plt.plot(x,y2)
plt.xlabel('Time in Seconds')
plt.ylabel('Magnitude')
plt.title('Scaled Data vs Time')
plt.subplots_adjust(hspace=0.5,wspace=0.35)
plt.show()



ser.close()
f.close()
