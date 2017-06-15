#include "CurieIMU.h"
#include "CurieTimerOne.h"

unsigned long SAMPLERATE = 5000; //10000 100Hz, 500 2000Hz, 1250 for pos

const float pi = 3.14159;
const float GyroRange=1000; //Gyro Range

const int AccRange=2; //Accelerometer Range

float dt=0.15;
int pitchgyro=0;
float rollgyro=0;
float yawgyro=0;
int final_roll;
int nextPosition;
int shoot;

int gyrox,gyroy,gyroz; //Raw data to read the gyro data into
int accelx,accely,accelz; // previous orientation (for comparison)

float analogReadVal;
unsigned long sampleTime;


unsigned long commandTimer = millis();

void setup() {
  Serial.begin(115200); // initialize Serial communication
  while (!Serial);    // wait for the serial port to open
  
  CurieIMU.begin();// initialize device
  CurieIMU.setAccelerometerRange(AccRange);// Set the accelerometer range to 2G
  CurieIMU.setGyroRange(GyroRange);
  CurieIMU.setGyroRate(800);
  CurieIMU.autoCalibrateGyroOffset();
  CurieIMU.autoCalibrateAccelerometerOffset(X_AXIS, 0);
  CurieIMU.autoCalibrateAccelerometerOffset(Y_AXIS, 0);
  CurieIMU.autoCalibrateAccelerometerOffset(Z_AXIS, 1);

  CurieTimerOne.start(SAMPLERATE, &sampleProcess);
}

int nextState = 0;
void loop() {
  const int commandDelay = 25; // Delay between serial outputs
  int lastPosition;
  int fireYes = 1;


  if(abs(nextPosition-lastPosition) > 60){
    while(millis() - commandTimer <= commandDelay);
    Serial.println(nextPosition);
    lastPosition = nextPosition;
    commandTimer = millis();
  }

  if(shoot) Serial.println(fireYes);
  
}

void sampleProcess(){
  CurieIMU.readAccelerometer(accelx,accely,accelz);
  CurieIMU.readGyro(gyrox, gyroy, gyroz);
  nextPosition = positionFunction();
  analogReadVal = analogRead(0);
  shoot = shootEMG();
}

int DegreesPerSecond(int rawValue){
 //Convert Raw Value from Gyro to DegreesPerSecond
  int DPS = (rawValue/32768.9)*GyroRange;
  return DPS;}

float toms2(int acc){
//Accelerometer Raw Data converted to meters/sec^2    
    float AccConverted = (acc/32768.0)*AccRange; 
    return AccConverted;}

int positionFunction() {//Used all the above functions to map the Angle of the board to a position in game
  const static int maxRoll = 1000;
  const static int minRoll = -1000;

  static unsigned long gyroLastSampleTime = micros();
  unsigned long gyroSampleTime = micros();
  
  
  //converts gyro values to degrees per second and scale down
  int gdx = DegreesPerSecond(gyrox);
  //Serial.println((float(gyroSampleTime-gyroLastSampleTime))/100000);
  //angular velocity is integrated over time to obtain pitch, roll and yaw from the Gyroscope Readings
  pitchgyro +=gdx*((float(gyroSampleTime-gyroLastSampleTime))/100000)*4;
  gyroLastSampleTime = gyroSampleTime;
  
  //pitch, roll and yaw are obtained from the Accelerometer Readings
  float roll = 30 * 180 * atan (float(accely)/sqrt(float(accelx)*float(accelx) + float(accelz)*float(accelz)))/pi;
  
  //converts accelerometer values to m/s^2
  float accelerationZ = toms2(accelz);
  
  //Complementery Filter is applied
  if(accelerationZ >= 0.0 && accelerationZ <= 1.0) pitchgyro = dynamicComp(pitchgyro,roll, pow(accelerationZ/(1.6),6));
 
  if (pitchgyro > maxRoll) pitchgyro = maxRoll;
  else if (pitchgyro < minRoll) pitchgyro = minRoll;
  final_roll = (pitchgyro - (minRoll))*(745-5)/(maxRoll - (minRoll)) + 5;  // Scale

  Serial.print(pitchgyro);
  Serial.print('\t');
  Serial.print(roll);
  Serial.print('\t');
  Serial.println(final_roll);
  
  return final_roll;
  
}

//Complementary Filter, takes a portion of GyroScope Data and Accelerometer Data
//To Compensate the Drift error by the Gyroscope, caused by integration.
float dynamicComp(float gyroInput, float AccInput, float Alpha){  
  float filtered_value = (1-Alpha)*gyroInput + Alpha*AccInput;
  return filtered_value;
}

int shootEMG(){
  int shootPress;
  static int shootCounter = 0;
  static int shootState = 0;
  static float maxEMG = 50;
  float hpFiltered = thirdOrder_HighPass(analogReadVal); //Scale EMG to Volts and Apply Highpass Filter
  float bpFiltered = thirdOrder_LowPass(hpFiltered);  // Apply Low Pass Filter
  float emgOutput = boxcarFilterSample(abs(bpFiltered)); // Apply Box Car Filter w/ Recitify Output
 
  if (emgOutput > maxEMG) {
    maxEMG = maxEMG+emgOutput/2;
  }
  else if (maxEMG > 50) maxEMG *= 0.9999;
  if(emgOutput >= 0.55*maxEMG){
    shootPress = 1;
  }
  
  else if (emgOutput < 0.35*maxEMG && (shootPress == 1))shootPress = 0;
  return shootPress;
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

float boxcarFilterSample(float sample) {
  static const int boxcarWidth = 16; // Change this value to alter boxcar length 
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

