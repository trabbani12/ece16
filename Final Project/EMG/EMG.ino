#include "CurieTimerOne.h"

unsigned long SAMPLERATE = 5000; //10000 100Hz

float analogReadVal;
unsigned long sampleTime;
int shootPress;

float output;
float average;

//int shootCounter;

void analogDataRead(){
  analogReadVal = analogRead(0);
}

void setup() {
  Serial.begin(250000);
  while(!Serial);
  Serial.println("Serial Comm. Established");
  CurieTimerOne.start(SAMPLERATE, &analogDataRead);
}

void loop() {
  shootEMG();
  Serial.print(output); // serial print box car output
  Serial.print("\t"); // Tab seperated
  Serial.print(average); // serial print box car output
  Serial.print("\t"); // Tab seperated
  Serial.println(shootPress); // serial print box car output
}

void shootEMG(){
  static int shootCounter = 0;
  static int shootState = 0;
  
  float hpFiltered = thirdOrder_HighPass(analogReadVal*(3.30)/1024.00); //Scale EMG to Volts and Apply Highpass Filter
  float bpFiltered = thirdOrder_LowPass(hpFiltered);  // Apply Low Pass Filter
  output = boxcarFilterSampleShort(abs(bpFiltered)); // Apply Box Car Filter w/ Recitify Output
  average = boxcarFilterSampleLong(output); // Apply Box Car Filter w/ Recitify Output
  
  if(output >= average && shootState == 0){
    shootCounter += 1;
    if(shootCounter >= 50){ // TUNING PARAMETER
      shootCounter = 0;
      shootPress = 1;
      shootState = 1;
    }}
  else if(output < average && shootState == 0 && shootCounter >= 0){
    shootCounter -= 1;}
  else if(shootState == 1){
    shootState = 0;
    shootPress = 0;}
}

float thirdOrder_HighPass(float sample) {
  static const float a[4] = {1, -2.37409474, 1.9293556, -0.53207537};
  static const float b[4] = {0.72944072, -2.18832217, 2.18832217, -0.72944072};
  // x array for holding recent inputs (newest input as index 0, delay of 1 at index 1, etc.
  static float x[4] = {0};
  // x array for holding recent inputs (newest input as index 0, delay of 1 at index 1, etc.
  static float y[4] = {0};
  x[0] = sample;
  // Calculate the output filtered signal based on a weighted sum of previous inputs/outputs
  y[0] = (b[0]*x[0]+b[1]*x[1]+b[2]*x[2]+b[3]*x[3])-(a[1]*y[1]+a[2]*y[2]+a[3]*y[3]); y[0] /= a[0];
  // Shift the input signals by one timestep to prepare for the next call to this function
  x[3] = x[2];
  x[2] = x[1];
  x[1] = x[0];
  // Shift the previously calculated output signals by one time step to prepare for the next call to this function
  y[3] = y[2];
  y[2] = y[1];
  y[1] = y[0];
  return y[0];}

float thirdOrder_LowPass(float sample) {
  static const float a[4] = {1.00, 0.00, -0.33333333, 0.00};
  static const float b[4] = {0.16666667, 0.5, 0.5, 0.16666667};
  // x array for holding recent inputs (newest input as index 0, delay of 1 at index 1, etc.
  static float x[4] = {0};
  // x array for holding recent inputs (newest input as index 0, delay of 1 at index 1, etc.
  static float y[4] = {0};
  x[0] = sample;
  // Calculate the output filtered signal based on a weighted sum of previous inputs/outputs
  y[0] = (b[0]*x[0]+b[1]*x[1]+b[2]*x[2]+b[3]*x[3])-(a[1]*y[1]+a[2]*y[2]+a[3]*y[3]); y[0] /= a[0];
  // Shift the input signals by one timestep to prepare for the next call to this function
  x[3] = x[2];
  x[2] = x[1];
  x[1] = x[0];
  // Shift the previously calculated output signals by one time step to prepare for the next call to this function
  y[3] = y[2];
  y[2] = y[1];
  y[1] = y[0];
  return y[0];}

float boxcarFilterSampleLong(float sample) {
static const int boxcarWidth = 100; // Change this value to alter boxcar length 
static float recentSamples[boxcarWidth] = {0}; // hold onto recent samples
static int readIndex = 0;// the index of the current reading
static float total = 0;// the running total
static float average = 0;// the average
  total = total - recentSamples[readIndex]; // subtract the last reading:
  recentSamples[readIndex] = sample; // add new sample to list (overwrite oldest sample) 
  total = total + recentSamples[readIndex]; // add the reading to the total
  readIndex = readIndex + 1; // advance to the next position in the array
  if (readIndex >= boxcarWidth) {// if we're at the end of the array...
    readIndex = 0;} // ...wrap around to the beginning
  average = total / boxcarWidth + 0.02; // calculate the average  //TUNING PARAMETER
  return average;} // send it to the computer as ASCII digits

float boxcarFilterSampleShort(float sample) {
static const int boxcarWidth = 100; // Change this value to alter boxcar length 
static float recentSamples[boxcarWidth] = {0}; // hold onto recent samples
static int readIndex = 0;// the index of the current reading
static float total = 0;// the running total
static float average = 0;// the average
  total = total - recentSamples[readIndex]; // subtract the last reading:
  recentSamples[readIndex] = sample; // add new sample to list (overwrite oldest sample) 
  total = total + recentSamples[readIndex]; // add the reading to the total
  readIndex = readIndex + 1; // advance to the next position in the array
  if (readIndex >= boxcarWidth) {// if we're at the end of the array...
    readIndex = 0;} // ...wrap around to the beginning
  average = total / boxcarWidth; // calculate the average
  return average;} // send it to the computer as ASCII digits

