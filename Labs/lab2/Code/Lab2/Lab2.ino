#include "CurieIMU.h"

unsigned long starting=0;
unsigned long ending=0;
int input;





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

}

void loop() {


  starting=micros();
  input=analogRead(A0);
  input=analogRead(A1);
  input=analogRead(A2);
  ending=micros()-starting;
  Serial.print("Analog: ");
  Serial.println(ending);

  float gx, gy, gz;
  

  starting=micros();
  CurieIMU.readGyroScaled(gx, gy, gz);
  ending=micros()-starting;
  Serial.print("Gyro: ");
  Serial.println(ending);


  starting=micros();
  CurieIMU.readAccelerometerScaled(gx, gy, gz);
  ending=micros()-starting;
  Serial.print("Accelerometer: ");
  Serial.println(ending);  
  

  delay(1000);
  

}
