import os
import serial   #import serial library
import re #used for spliting white spaces
import datetime
import numpy as np
import matplotlib.pyplot as plt
import scipy.signal
import matplotlib.animation as animation






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
arr1=[0]
arr2=[0]

fig, ax = plt.subplots()
line, = ax.plot([], [], lw=2)
ax.grid()
xdata, ydata = [], []


counter=0;

while(int(counter)<5000):
    s=ser.readline()
    f.write(s)
    counter=counter+1
f.close()


fs = open(text_name + ".txt","r")
counter=0
fig = plt.figure()
ax1 = fig.add_subplot(1,1,1)

while(int(counter)<100):

    def animate(i):
        for i in range(20):
            s = fs.readline()
            words = s.split()

            word[0]=words[0]
            a = float(words[1])
            a = a/1000000.0
            b = float(words[2])
            data1.append(data1[i] + a)
            data2.append(b)
            temp=(b/1024)
            temp=float(3.3*temp)
            temp=(temp - 1.5)/3600
            datascaled.append((temp))

        b_high, a_high = scipy.signal.butter(3, 0.1, 'high', analog=False)
        signal_out = scipy.signal.lfilter(b_high, a_high, datascaled)
        #low pass filter
        b_low, a_low = scipy.signal.butter(3, .5, 'low', analog=False)
        signal_out1 = scipy.signal.lfilter(b_low, a_low, signal_out)


        #box filter
        box = scipy.signal.boxcar(100)
        signal_out2 = scipy.signal.lfilter(box, 1, signal_out1)
        y3=np.array(signal_out2)



        datatime=[0]
        dataprocessed=[0]

        plt.ion()
        if (int(counter)>19):
            arr1.extend(data1[(counter-20)*20:((counter+1)*20)-1])
            arr2.extend(signal_out2[(counter-20)*20:((counter+1)*20)-1])
            arr1[0:399]=arr1[(counter-20)*20:((counter+1)*20)-1]
            arr2[0:399]=arr2[(counter-20)*20:((counter+1)*20)-1]
        else:
            arr1.extend(data1[0:(counter+1)*20])
            arr2.extend(signal_out2[0:(counter+1)*20])
        z1=np.array(arr1)
        z2=np.array(arr2)
        ax1.clear()
        ax1.plot(z1,z2)
        #plt.pause(0.1)
        #plt.hold(False)
        plt.show()
    counter=counter+1
    ani=animation.FuncAnimation(fig, animate, interval=1000)
    #plt.show()
    #plt.show()










ser.close()
fs.close()
