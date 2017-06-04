#include "CurieIMU.h"
#include "CurieTimerOne.h"

unsigned long starting=0;
unsigned long ending=0;
unsigned long temp=0;
int input1, input2, input3, counter=1;
int time1=10000;
char user = '0';


//#define SERIAL_PORT_LOG_ENABLE 1

void timedAnalog()   // callback function when interrupt is asserted
{
  
  Serial.print(counter++);
  
 // starting=micros();
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
  starting=micros();
  
  
  }
  

void timedGyro()   // callback function when interrupt is asserted
{
  Serial.print(counter++);
  int gx, gy, gz;
//  starting=micros();
  CurieIMU.readGyro(gx, gy, gz);
  ending=micros()-starting;
  //temp=ending;
  Serial.print("    ");
  Serial.print(ending);
  Serial.print("    ");
  Serial.print(gx);
  Serial.print("    ");
  Serial.print(gy);
  Serial.print("    ");
  Serial.println(gz);
  starting=micros();
   
  }


void timedAcc()   // callback function when interrupt is asserted
{


  Serial.print(counter++);
  int gx, gy, gz;
  //starting=micros();
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
  starting=micros();
  }
  
  
  
  
//}

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
while(Serial.available()==0)
{}
user = Serial.read();
Serial.println(user);
if(user=='1'){
CurieTimerOne.start(time1, &timedAnalog);
starting=micros();
}

if(user=='2'){
CurieTimerOne.start(time1, &timedGyro);
starting=micros();
}

if(user=='3'){
CurieTimerOne.start(time1, &timedAcc);
starting=micros();
}
}

void loop() {
}

  
  


