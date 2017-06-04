#include "CurieIMU.h"
#include "CurieTimerOne.h"

unsigned long starting=0;
unsigned long ending=0;
unsigned long temp=0;
int input1, input2, input3, counter=1;
int time1=5000;//200Hz
char user = '0';


//#define SERIAL_PORT_LOG_ENABLE 1

void timedAnalog()   // callback function when interrupt is asserted
{
  
  Serial.print(counter++);
  
 // starting=micros();
  input1=analogRead(A0);

  ending=micros()-starting;
//  Serial.print("Analog: ");
//  Serial.println(ending);
  Serial.print("    ");
  Serial.print(ending);
  Serial.print("    ");
  Serial.println(input1);

  starting=micros();
  
  
  }
  
  
  
  
  
//}

void setup() {
  // put your setup code here, to run once:
 
Serial.begin(115200);
while (!Serial);
pinMode(A0,INPUT);

CurieIMU.begin();


Serial.println("Press any button to Start Sampling");
while(Serial.available()==0)
{}

CurieTimerOne.start(time1, &timedAnalog);
starting=micros();



}

void loop() {
}

  
  


