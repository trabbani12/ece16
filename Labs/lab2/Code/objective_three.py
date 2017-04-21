import os
import serial   #import serial library

#ser = serial.Serial("/dev/cu.usbmodem1411") # create serial object
                                            # pointing to arduino connection
print ("*For all decisions, enter the number associated with the choice you would like to make*")
text_name = raw_input("Enter the name of the text file you would like to output to: ")

path = os.path.realpath(__file__) #get the path of the currently executing python file

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

decision = raw_input("Which would you like to record?\n1) Analog\n2) Accelerometer\n3) Gyroscope\n")

ser.write(int(decision))

while(1):
    f.write(ser.readline())

ser.close()
f.close()
