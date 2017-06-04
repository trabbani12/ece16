import os
import serial   #import serial library
import re #used for spliting white spaces

ser = serial.Serial("/dev/cu.usbmodem1421", 115200) # create serial object
                                            # pointing to arduino connection

print ("*For all decisions, enter the number associated with the choice you would like to make*")
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

decision = raw_input("Which would you like to record?\n1) Analog\n2) Gyroscope\n3) Accelerometer\n")

print ser.readline()

ser.write(decision)
ser.readline()
word = [0,0,0,0,0]

counter=0;
while(int(word[0])<5000):
    self.s = ser.readline()
    #counter=counter++
    words = s.split()
    #print type(words)
    #print len(words)
    #print words
    #print s
    if(int(decision) == 1): #analog converstion
        word[0]=words[0]
        a = float(words[1])
        b = a/1000.0
        word[1] = b
        a = float(words[2])
        b = a/1024.0
        c = b*(3.3)
        word[2] = c
        a = float(words[3])
        b = a/1024.0
        c = b*(3.3)
        word[3] = c
        a = float(words[4])
        b = a/1024
        c = b*(3.3)
        word[4] = c
        print word
        f.write(str(word[0]) + "\t" + str(word[1]) + "\t" + str(word[2]) + "\t" + str(word[3]) + "\t" + str(word[4]) + "\n")

    if(int(decision) == 3): #Accelerometer converstion
        word[0]=words[0]
        a = float(words[1])
        b = a/1000.0
        word[1] = b
        a = float(words[2])
        b = a/327680000.0
        c = b*(2)
        word[2] = c
        a = float(words[3])
        b = a/327680000.0
        c = b*(2)
        word[3] = c
        a = float(words[4])
        b = a/327680000.0
        c = b*(2)
        word[4] = c
        print word
        f.write(str(word[0]) + "\t" + str(word[1]) + "\t" + str(word[2]) + "\t" + str(word[3]) + "\t" + str(word[4]) + "\n")

    if(int(decision) == 2): #Gyroscope converstion
        word[0]=words[0]
        a = float(words[1])
        b = a/1000.0
        word[1] = b
        a = float(words[2])
        b = a/32768.9
        c = b*(250)
        word[2] = c
        a = float(words[3])
        b = a/32768.9
        c = b*(250)
        word[3] = c
        a = float(words[4])
        b = a/32768.9
        c = b*(250)
        word[4] = c
        print word
        f.write(str(word[0]) + "\t" + str(word[1]) + "\t" + str(word[2]) + "\t" + str(word[3]) + "\t" + str(word[4]) + "\n")

ser.close()
f.close()
