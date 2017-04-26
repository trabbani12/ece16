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
user=Serial.read();

}

void loop() {
  
  if (user=='1') {
  while(1) {
  
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
  Serial.println(input3);
  delay((10000-ending));
  }
  }
  
  
  
  if (user=='2') {
  while(1) {
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
  Serial.println(gz);
  delay((10000-ending));
  }
  }


  if (user=='3') {
  while(1) {
  Serial.print(counter++);
  int gx, gy, gz;
  starting=micros();
  CurieIMU.readAccelerometer(gx, gy, gz);
  ending=micros()-starting;
  Serial.print("    ");
  Serial.print(ending);
  Serial.print("    ");
  Serial.print(gx);
  Serial.print("    ");
  Serial.print(gy);
  Serial.print("    ");
  Serial.println(gz);
  delay((10000-ending));
  }
  
  }
  
  delay(1000);
  

}
