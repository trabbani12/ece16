

#include "CurieIMU.h"


const float pi = 3.14159;
const float GyroRange=125.0;//Gyro Range
const float AccRange=2.0;//Accelerometer Range

int shoot=1;//Shoot Command
int gyrox,gyroy,gyroz; //Raw data to read the gyro data into
float dt=0.15;
float pitchgyro=0;
float rollgyro=0;
float yawgyro=0;
int x,y,z;
// previous orientation (for comparison)

void setup() {
  Serial.begin(115200); // initialize Serial communication
  while (!Serial);    // wait for the serial port to open

  // initialize device

  CurieIMU.begin();

  // Set the accelerometer range to 2G
  CurieIMU.setAccelerometerRange(AccRange);
  CurieIMU.setGyroRange(GyroRange);
  CurieIMU.noGyroOffset();
  CurieIMU.noAccelerometerOffset();
  
  
}

float CompFilter(float gyroInput, float AccInput){
  //Complementary Filter, takes a portion of GyroScope Data and Accelerometer Data
  //To Compensate the Drift error by the Gyroscope, caused by integration.
  float filtered_value = 0.95*gyroInput + 0.05*AccInput;
  return filtered_value;
}

float DegreesPerSecond(int rawValue){
 //Convert Raw Value from Gyro to DegreesPerSecond
  float DPS = ( rawValue/32768.9)*GyroRange;
  return DPS;
}


float toms2(int acc){
//Accelerometer Raw Data converted to meters/sec^2    
    float AccConverted = (acc/32768000.0)*AccRange*9.8; 
    return AccConverted;
}


void positionFunction() {//Used all the above functions to map the Angle of the board to a position in game
  CurieIMU.readAccelerometer(x, y,z);
  CurieIMU.readGyro(gyrox, gyroy, gyroz);
//converts gyro values to degrees per second
  float gdx = DegreesPerSecond(gyrox);
  float gdy = DegreesPerSecond(gyroy);
  float gdz = DegreesPerSecond(gyroz);



//converts accelerometer values to m/s^2
  float accelerationX = toms2(x);
  float accelerationY = toms2(y);
  float accelerationZ = toms2(z);

//pitch, roll and yaw are obtained from the Accelerometer Readings
  float pitch = 180 * atan (accelerationX/sqrt(accelerationY*accelerationY + accelerationZ*accelerationZ))/pi;
  float roll = 180 * atan (accelerationY/sqrt(accelerationX*accelerationX + accelerationZ*accelerationZ))/pi;
  float yaw = 180 * atan (accelerationZ/sqrt(accelerationX*accelerationX + accelerationZ*accelerationZ))/pi;


  //angular velocity is integrated over time to obtain pitch, roll and yaw from the Gyroscope Readings
  pitchgyro +=(gdx)*dt;
  rollgyro -=(gdy)*dt;
  yawgyro +=(gdz)*dt;   
  
  //Complementery Filter is applied
  float final_roll = CompFilter(pitchgyro,roll);
  //float final_pitch = CompFilter(rollgyro,pitch);
  //float final_yaw = CompFilter(yawgyro,yaw);

 
    if (final_roll<0) {
      int fposition=(abs(final_roll)/GyroRange)*375;
      Serial.println(fposition+370);
      //delay(100);
    }
    else{
      int fposition=(final_roll/GyroRange)*375;
      Serial.println(370-fposition);
      //delay(100);
    }
  
   
}


void loop() {

  positionFunction();
  //Serial.println(shoot);
  delay(50);
  
}

