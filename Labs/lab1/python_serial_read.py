import serial   #import serial library

ser = serial.Serial("/dev/cu.usbmodem1411") # create serial object
                                            # pointing to arduino connection
print ser.name # used to validate correct port to Arudino
ser.write("Hello") #input to Serial Monitor used to initiate LED Blinking

while(1): #as long as the arduino prints to serial monitor, run this loop
  print ser.readline() #print each line of the serial monitor to terminal screen

ser.close() #used to close objects. Since while loop never ends this would
            #never be reached, but good practice.
