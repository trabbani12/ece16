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
        exit()                                        # pointing to arduino connection

decision = raw_input("Enter Any Character to start reading from serial port\n")
ser.readline()


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

#        word[3]=datetime.datetime.now().strftime('%Y-%m-%d %H:%M:%S')
    data1.append(data1[counter] + a)
    data2.append(b)
    temp=(b/1024)
    temp=float(3.3*temp)
    temp=(temp - 1.5)/3600
    datascaled.append(temp)
    counter=counter+1





x=np.array(data1)
y=np.array(data2)
#plt.plot(x,y)
#plt.show()
y2=np.array(datascaled)
plt.subplot(3,1,1)
plt.plot(x,y2)
plt.title("Unfiltered Data")
plt.xlabel("Time in Seconds")
plt.ylabel("Voltage")






b_high, a_high = scipy.signal.butter(3, 0.1, 'high', analog=False)
signal_out = scipy.signal.lfilter(b_high, a_high, datascaled)



b_low, a_low = scipy.signal.butter(3, .5, 'low', analog=False)

onlyLP= scipy.signal.lfilter(b_low, a_low, datascaled)

signal_out1 = scipy.signal.lfilter(b_low, a_low, signal_out)
plt.figure(0)
plt.subplot(4,1,1)
plt.plot(x,datascaled)
plt.title("Data without Filtering")

plt.ylabel("Voltage")
plt.subplot(4,1,2)
plt.plot(x,signal_out)
plt.title("High Pass Only")
plt.subplot(4,1,3)
plt.plot(x,onlyLP)
plt.title("Low Pass Only")

plt.subplot(4,1,4)
plt.plot(x,signal_out1)
plt.title("Band Pass")
plt.xlabel("Time in Seconds")

plt.subplots_adjust(hspace=1,wspace=0.2)


recsignal=[abs(i) for i in signal_out1]
box = scipy.signal.boxcar(100)
signal_out2 = scipy.signal.lfilter(box, 1, recsignal)

y3=np.array(signal_out2)
y4=np.array(recsignal)
plt.figure(1)
plt.subplot(3,1,2)
plt.plot(x,y4)
plt.title("Rectified Signal")
plt.xlabel("Time in Seconds")
plt.ylabel("Voltage")
plt.subplot(3,1,3)
plt.plot(x,y3)
plt.title("Smoothing Operation Performed")
plt.xlabel("Time in Seconds")
plt.ylabel("Voltage")
plt.subplots_adjust(hspace=0.7,wspace=0.35)
plt.show()



f.close()
ser.close()
