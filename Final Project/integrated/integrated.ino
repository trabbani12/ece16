#include "CurieIMU.h"
#include "CurieTimerOne.h"

unsigned long SAMPLERATE = 10000; //10000 100Hz, 500 2000Hz

const float pi = 3.14159;
const float GyroRange=2000; //Gyro Range

const int AccRange=2; //Accelerometer Range

int shoot=1;//Shoot Command
int gyrox,gyroy,gyroz; //Raw data to read the gyro data into
float dt=0.15;
float pitchgyro=0;
float rollgyro=0;
float yawgyro=0;
int x,y,z;
// previous orientation (for comparison)

float analogReadVal;
unsigned long sampleTime;
int shootPress;
unsigned long commandTimer = millis();

void setup() {
  
  Serial.begin(115200); // initialize Serial communication
  while (!Serial);    // wait for the serial port to open
  
  CurieIMU.begin();// initialize device

  // Set the accelerometer range to 2G
  CurieIMU.setAccelerometerRange(AccRange);
  CurieIMU.setGyroRange(GyroRange);
  CurieIMU.setGyroRate(200);
  CurieIMU.autoCalibrateGyroOffset();
  CurieIMU.autoCalibrateAccelerometerOffset(X_AXIS, 0);
  CurieIMU.autoCalibrateAccelerometerOffset(Y_AXIS, 0);
  CurieIMU.autoCalibrateAccelerometerOffset(Z_AXIS, 1);

  CurieTimerOne.start(SAMPLERATE, &analogDataRead);
  CurieTimerOne.attachInterrupt(&imuDataRead);
  delay(1000);
}

int nextState = 0;
void loop() {
  const int commandDelay = 10; // Delay between serial outputs
  int nextPosition = positionFunction();
  //int shoot = shootEMG();

  
  if ((millis() - commandTimer > commandDelay) && nextState == 0){
    //int nextPosition = positionFunction();
    Serial.println(runningAverage(nextPosition));
    commandTimer = millis();
    nextState = 1;
  }
  if ((millis() - commandTimer > commandDelay) && nextState == 1){
    //Serial.println(shoot);
    nextState = 0;
    commandTimer = millis();
  }
}

void analogDataRead(){
  analogReadVal = analogRead(0);
}

void imuDataRead(){
  CurieIMU.readAccelerometer(x,y,z);
  CurieIMU.readGyro(gyrox, gyroy, gyroz);
}

float DegreesPerSecond(int rawValue){
 //Convert Raw Value from Gyro to DegreesPerSecond
  float DPS = ( rawValue/32768.9)*GyroRange;
  return DPS;}

float toms2(int acc){
//Accelerometer Raw Data converted to meters/sec^2    
    float AccConverted = (acc/32768.0)*AccRange; 
    return AccConverted;}


int positionFunction() {//Used all the above functions to map the Angle of the board to a position in game
  float final_roll;
  
  static float maxRoll = 2000;
  static float minRoll = -2000;
//converts gyro values to degrees per second
  int gdx = DegreesPerSecond(gyrox);
  
//converts accelerometer values to m/s^2
  float accelerationX = toms2(x);
  float accelerationY = toms2(y);
  float accelerationZ = toms2(z);

//pitch, roll and yaw are obtained from the Accelerometer Readings
  float roll = 180 * atan (accelerationY/sqrt(accelerationX*accelerationX + accelerationZ*accelerationZ))/pi;

  
  //pitchgyro *= 0.9; //decay
  gdx /=4; // Scale it down
  
  
  //angular velocity is integrated over time to obtain pitch, roll and yaw from the Gyroscope Readings
  pitchgyro +=(float(gdx))*dt; 

  //Complementery Filter is applied
  pitchgyro = dynamicComp(pitchgyro,roll*25, accelerationZ/2);
  //Serial.print(pitchgyro);
  //Serial.print('\t');
  //Serial.print(roll*25);
  //Serial.print('\t');
  
  //Complementery Filter is applied
  //final_roll = CompFilter(pitchgyro,roll*25);
  final_roll = pitchgyro;

  if (pitchgyro > maxRoll) pitchgyro = maxRoll;
  else if (pitchgyro < minRoll) pitchgyro = minRoll;
  
  final_roll = (pitchgyro - (minRoll))*(745-5)/(maxRoll - (minRoll)) + 5;  // Scale
  //Serial.println(final_roll);
  return final_roll;
}

float dynamicComp(float gyroInput, float AccInput, float Alpha){
  //Complementary Filter, takes a portion of GyroScope Data and Accelerometer Data
  //To Compensate the Drift error by the Gyroscope, caused by integration.
  float filtered_value = (1-Alpha)*gyroInput + Alpha*AccInput;
  return filtered_value;
  //return runningAverage(filtered_value);
}

long runningAverageLong(int M) {
  #define LM_SIZE 30
  static int LM[LM_SIZE];      // LastMeasurements
  static byte index = 0;
  static long sum = 0;
  static byte count = 0;

  // keep sum updated to improve speed.
  sum -= LM[index];
  LM[index] = M;
  sum += LM[index];
  index++;
  index = index % LM_SIZE;
  if (count < LM_SIZE) count++;
  return sum / count;
}

long runningAverage(int M) {
  #define LM_SIZE 20
  static int LM[LM_SIZE];      // LastMeasurements
  static byte index = 0;
  static long sum = 0;
  static byte count = 0;

  // keep sum updated to improve speed.
  sum -= LM[index];
  LM[index] = M;
  sum += LM[index];
  index++;
  index = index % LM_SIZE;
  if (count < LM_SIZE) count++;
  return sum / count;
}

float CompFilter(float gyroInput, float AccInput){
  //Complementary Filter, takes a portion of GyroScope Data and Accelerometer Data
  //To Compensate the Drift error by the Gyroscope, caused by integration.
  float filtered_value = 0.8*gyroInput + 0.2*AccInput;
  //float filtered_value = 0.95*gyroInput;  
  return filtered_value;
}

int shootEMG(){
  static int shootCounter = 0;
  static int shootState = 0;
  
  float hpFiltered = thirdOrder_HighPass(analogReadVal*(3.30)/1024.00); //Scale EMG to Volts and Apply Highpass Filter
  float bpFiltered = thirdOrder_LowPass(hpFiltered);  // Apply Low Pass Filter
  float output = boxcarFilterSample(abs(bpFiltered)); // Apply Box Car Filter w/ Recitify Output
  float average = boxcarFilterSample(output); // Apply Box Car Filter w/ Recitify Output
  
  if(output >= average && shootState == 0){
    shootCounter += 1;
    if(shootCounter >= 80){ // TUNING PARAMETER
      shootCounter = 0;
      shootPress = 1;
      shootState = 1;
      return shootPress;
    }}
  else if(output < average && shootState == 0 && shootCounter >= 0){
    shootCounter -= 5;}
  else if(shootState == 1){
    shootState = 0;
    shootPress = 0;
    return shootPress;
  }
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
    readIndex = 0;
  } // ...wrap around to the beginning
  average = total / boxcarWidth + 0.03; // calculate the average  //TUNING PARAMETER
  return average;
} // send it to the computer as ASCII digits

