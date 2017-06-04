#include "CurieIMU.h"
#include "CurieTimerOne.h"

unsigned long starting=0;
unsigned long ending=0;
unsigned long temp=0, st1=0, en1=0;
float input1, input2, input3, counter=1;
int time1=5000;//200Hz
float scaled, HP,LP,BP,box, REC,scaled2;
char user = '0';


float boxcarFilter(float sample)
{
  static const int boxcarWidth = 10; // Change this value to alter boxcar length
  static float recentSamples[boxcarWidth] = {0}; // hold onto recent samples
  static int readIndex = 0;              // the index of the current reading
  static float total = 0;                  // the running total
  static float average = 0;                // the average

  // subtract the last reading:
  total = total - recentSamples[readIndex];
  // add new sample to list (overwrite oldest sample)
  recentSamples[readIndex] = sample;
  // add the reading to the total:
  total = total + recentSamples[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= boxcarWidth) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / boxcarWidth;
  // send it to the computer as ASCII digits
  return average;
}


float highpass(float sample)
{
  static const float a[4] = {-2.37409474, 1.9293, -0.53207, 0}; // ADD A VALUES HERE
  static const float b[4] = {0.722944, -2.18832, 2.18832, -0.722944};// ADD B VALUES HERE

  // x array for holding recent inputs (newest input as index 0, delay
//of 1 at index 1, etc.
  static float x[4] = {0};
  // x array for holding recent inputs (newest input as index 0, delay
//of 1 at index 1, etc.
  static float y[4] = {0};

  x[0] = sample;

  // Calculate the output filtered signal based on a weighted sum of
//previous inputs/outputs
  y[0] = (b[0]*x[0]+b[1]*x[1]+b[2]*x[2]+b[3]*x[3])-(a[1]*y[1]+a[2]*y[2]+a[3]*y[3]);
  y[0] /= a[0];

  // Shift the input signals by one timestep to prepare for the next
//call to this function
  x[3] = x[2];
  x[2] = x[1];
  x[1] = x[0];

  // Shift the previously calculated output signals by one time step
//to prepare for the next call to this function
  y[3] = y[2];
  y[2] = y[1];
  y[1] = y[0];

  return y[0];
}

float lowpass(float sample)
{
  static const float a[4] = {1.000000, -2.775557e-16, 3.3333e-1, -1.850391e-17}; // ADD A VALUES HERE
  static const float b[4] = {0.16666667, 0.5, 0.5, 0.16666667};// ADD B VALUES HERE

  // x array for holding recent inputs (newest input as index 0, delay
//of 1 at index 1, etc.
  static float x[4] = {0};
  // x array for holding recent inputs (newest input as index 0, delay
//of 1 at index 1, etc.
  static float y[4] = {0};

  x[0] = sample;

  // Calculate the output filtered signal based on a weighted sum of
//previous inputs/outputs
  y[0] = (b[0]*x[0]+b[1]*x[1]+b[2]*x[2]+b[3]*x[3])-(a[1]*y[1]+a[2]*y[2]+a[3]*y[3]);
  y[0] /= a[0];

  // Shift the input signals by one timestep to prepare for the next
//call to this function
  x[3] = x[2];
  x[2] = x[1];
  x[1] = x[0];

  // Shift the previously calculated output signals by one time step
//to prepare for the next call to this function
  y[3] = y[2];
  y[2] = y[1];
  y[1] = y[0];

  return y[0];
}

void timedAnalog()   // callback function when interrupt is asserted
{
  

  input1=analogRead(A0);
  ending=micros()-starting;
  Serial.print(ending);
  Serial.print("    ");
  Serial.print(input1);
  Serial.print("    ");
  scaled=(input1/1024.0*3.3-1.5)/3.6;

  Serial.print(scaled);
  st1=micros();
  HP=highpass(scaled);
  Serial.print("    ");
  Serial.print(HP);
  LP=lowpass(scaled);
  Serial.print("    ");
  Serial.print(LP);
  BP=lowpass(LP);
  Serial.print("    ");
  Serial.print(BP);
  REC=abs(BP);
  Serial.print("    ");
  Serial.print(REC);
  box=boxcarFilter(REC);
  Serial.print("    ");
  Serial.print(box);
  en1=micros()-st1;
  Serial.print("    ");
  Serial.println(en1);
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
starting=micros();
CurieTimerOne.start(time1, &timedAnalog);
starting=micros();



}

void loop() {
}

  
  


