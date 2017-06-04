

#include "CurieIMU.h"
const float pi = 3.14159;
int lastOrientation = - 1;
int shoot=1;
// previous orientation (for comparison)

void setup() {
  Serial.begin(115200); // initialize Serial communication
  while (!Serial);    // wait for the serial port to open

  // initialize device

  CurieIMU.begin();

  // Set the accelerometer range to 2G
  CurieIMU.setAccelerometerRange(2);
}

void loop() {


  
int orientation = - 1;   // the board's orientation
  String orientationString; // string for printing description of orientation

  int x = CurieIMU.readAccelerometer(X_AXIS);
  int y = CurieIMU.readAccelerometer(Y_AXIS);
  int z = CurieIMU.readAccelerometer(Z_AXIS);


  float gx = (x/32768000.0)*2.0*9.8; 
  float gy = (y/32768000.0)*2.0*9.8;
  float gz = (z/32768000.0)*2.0*9.8; 


  float accelerationX = gx;
  float accelerationY = gy;
  float accelerationZ = gz;
  float pitch = 180 * atan (accelerationX/sqrt(accelerationY*accelerationY + accelerationZ*accelerationZ))/pi;
  float roll = 180 * atan (accelerationY/sqrt(accelerationX*accelerationX + accelerationZ*accelerationZ))/pi;
  float yaw = 180 * atan (accelerationZ/sqrt(accelerationX*accelerationX + accelerationZ*accelerationZ))/pi;

/*
  Serial.print("pitch ");
  Serial.println(pitch);
  Serial.print("roll ");
  Serial.println(roll);
  Serial.print("yaw ");
  Serial.println(yaw);
*/
  if (roll<0) {
    int fposition=(abs(roll)/90)*375;
    Serial.println(fposition+370);
    delay(100);
  }
  else{
    int fposition=(roll/90)*375;
    Serial.println(370-fposition);
    delay(100);
  }
  Serial.println(shoot);
  delay(50);
  

  

  
 
}



