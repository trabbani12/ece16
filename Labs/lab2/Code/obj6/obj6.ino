#include "CurieIMU.h"
#include "CurieTimerOne.h"

int time1=120000;
int orientation = - 1;
int lastOrientation = -1;
String Ostatus;


//#define SERIAL_PORT_LOG_ENABLE 1



void timedAcc()   // callback function when interrupt is asserted
{
  int gx, gy, gz;
  CurieIMU.readAccelerometer(gx, gy, gz);
  int absX = abs(gx);
  int absY = abs(gy);
  int absZ = abs(gz);
  
  
 if ( (absY > absX) && (absY > absZ)) {
    if (gy > 0) {
      Ostatus = "Landscape-1";
      orientation = 1;
    } else {
      Ostatus = "Landscape-2";
      orientation = 2;
    }
  } else {
    if (gx < 0) {
      Ostatus = "Portrait-1";
      orientation = 3;
    } else {
      Ostatus = "Portrait-2";
      orientation = 4;
    }
  }
  if (orientation != lastOrientation) {
    Serial.println(Ostatus);
    lastOrientation = orientation;
  }
  
}
  
//}


void setup() {
  // put your setup code here, to run once:
 
Serial.begin(115200);
while (!Serial);



CurieIMU.begin();
CurieIMU.setAccelerometerRange(2);

Serial.println("Press Any key to start the program");
while(Serial.available()==0)
{}



CurieTimerOne.start(time1, &timedAcc);

}

void loop() {
}

  
  


