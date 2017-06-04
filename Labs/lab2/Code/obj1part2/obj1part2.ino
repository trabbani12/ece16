#include "CurieIMU.h"

unsigned long starting=0;
unsigned long ending=0;
int input1, input2, input3, counter=1;
char user;





void setup() {
  // put your setup code here, to run once:
 
Serial.begin(115200);
while (!Serial);
pinMode(A0,INPUT);
pinMode(A1,INPUT);
pinMode(A2,INPUT);


CurieIMU.begin();
CurieIMU.setGyroRange(250);
CurieIMU.setAccelerometerRange(2);

Serial.println("Please make a selection");
/*
Serial.println("Analog:         Press 1");
Serial.println("Gyroscope:      Press 2");
Serial.println("Accelerometer:  Press 3");
*/
}

void loop() {
  
  user=Serial.read();
  if (user=='1') {
  while(1) {
  Serial.print(counter++);
  starting=micros();
  input1=analogRead(A0);
  input2=analogRead(A1);
  input3=analogRead(A2);
  delayMicroseconds(500);
  ending=micros()-starting;
  Serial.print("    ");
  Serial.print(ending);
  Serial.print("    ");
  Serial.print(input1);
  Serial.print("    ");
  Serial.print(input2);
  Serial.print("    ");
  Serial.println(input3);
  }
  }
  
  
  
  if (user=='2') {
  while(1) {
  Serial.print(counter++);
  int gx, gy, gz;
  starting=micros();
  CurieIMU.readGyro(gx, gy, gz);
  delayMicroseconds(9920);
  ending=micros()-starting;
  Serial.print("    ");
  Serial.print(ending);
  Serial.print("    ");
  Serial.print(gx);
  Serial.print("    ");
  Serial.print(gy);
  Serial.print("    ");
  Serial.println(gz);

  }
  }


  if (user=='3') {
  while(1) {
  Serial.print(counter++);
  int gx, gy, gz;
  starting=micros();
  CurieIMU.readAccelerometer(gx, gy, gz);
  delayMicroseconds(9920);
  ending=micros()-starting;
  Serial.print("    ");
  Serial.print(ending);
  Serial.print("    ");
  Serial.print(gx);
  Serial.print("    ");
  Serial.print(gy);
  Serial.print("    ");
  Serial.println(gz);

  }
  
  }
  

  

}
