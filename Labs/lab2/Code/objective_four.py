import os
import serial   #import serial library
import re #used for spliting white spaces
import numpy


path = os.path.realpath(__file__)

filename="Interruptdata"
avgcount=0
temp=0
missingdata=0
data1=[]
data2=[]
data3=[]
data4=[]


f = open(filename + ".txt","r")
count=0
linestatus=1
while(linestatus==1):
    #f.readline()
    s=f.readline()
    if (s != ''):

        parameters=s.split()

        data1.append(float(parameters[1]))
        data2.append(float(parameters[2]))
        data3.append(float(parameters[3]))
        data4.append(float(parameters[4]))
        count=count+1

        avgcount=float(avgcount)+float(parameters[1])
        if(int(temp+1) != int(parameters[0])):#checks to see if there is any
        #missing data
            print temp
            print parameters[0]
            missingdata=1
            missingdataline=parameters[0]
            #status is changed, line is saved to report.
        temp=temp+1
    else:
        linestatus=0
    averagetime=float(avgcount)/float(count)


a=numpy.array(data1)
stddev=numpy.std(a)
print 'Standard deviation: '
print stddev
#standard deviation

mean=numpy.mean(a)
print 'Mean: '
print mean
#mean

mintime=numpy.min(a)
print 'Min time: '
print mintime
#min time

maxtime=numpy.max(a)
print 'Max time: '
print maxtime
#maxtime

#missing data status
print 'Number of missing data:'
print missingdata
