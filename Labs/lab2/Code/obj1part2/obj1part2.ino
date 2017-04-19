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

Serial.println("Make a selection");

}

void loop() {
  user=Serial.read();
  if (user=='1') {
  Serial.print(counter++);
  starting=micros();
  input1=analogRead(A0);
  input2=analogRead(A1);
  input3=analogRead(A2);
  ending=micros()-starting;
//  Serial.print("Analog: ");
//  Serial.println(ending);
  Serial.print("    ");
  Serial.print(ending);
  Serial.print("    ");
  Serial.print(input1);
  Serial.print("    ");
  Serial.print(input2);
  Serial.print("    ");
  Serial.print(input3);
  
  
  }
  
  
  
  if (user=='2') {
  Serial.print(counter++);
  int gx, gy, gz;
  starting=micros();
  CurieIMU.readGyro(gx, gy, gz);
  ending=micros()-starting;
  Serial.print("    ");
  Serial.print(ending);
  Serial.print("    ");
  Serial.print(gx);
  Serial.print("    ");
  Serial.print(gy);
  Serial.print("    ");
  Serial.print(gz);
  
  }


  if (user=='3') {
  int gx, gy, gz;
  starting=micros();
  CurieIMU.readAccelerometer(gx, gy, gz);
  ending=micros()-starting;
  Serial.print("Accelerometer: ");
  Serial.println(ending);  
  
  }
  
  delay(1000);
  

}
