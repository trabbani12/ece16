import serial
ser = serial.Serial("/dev/cu.usbmodem1411")
print ser.name
ser.write("Hello")
while(1):
  print ser.readline()
ser.close()
