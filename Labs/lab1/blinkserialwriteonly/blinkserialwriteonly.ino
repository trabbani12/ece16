/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the UNO, MEGA and ZERO 
  it is attached to digital pin 13, on MKR1000 on pin 6. LED_BUILTIN is set to
  the correct LED pin independent of which board is used.
  If you want to know what pin the on-board LED is connected to on your Arduino model, check
  the Technical Specs of your board  at https://www.arduino.cc/en/Main/Products
  
  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
  
  modified 2 Sep 2016
  by Arturo Guadalupi
  
  modified 8 Sep 2016
  by Colby Newman
*/
int incomingbyte=0; //global input varaible declartion 

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT); //declaring on-board LED as output 
  Serial.begin(9600); //establish baud rate 
  while(!Serial); //wait for Serial Monitor to be ready for use before continuing to program 
  Serial.print("Welcome, the Board has been reset\n"); //Print welcome message 
  Serial.print("Input a character to continue\n"); //ask for input 
} 

// the loop function runs over and over again forever
void loop() {
  incomingbyte=Serial.read(); //receive input
  while(incomingbyte>0){ //as long as the input was greater than 0, lights would then being to blink
    Serial.print("LED is ON\n"); //print to serial monitor that LED is ON
    digitalWrite(LED_BUILTIN, HIGH);// turn the LED on (HIGH is the voltage level)
    delay(1000);    // wait for a second
    Serial.print("LED is OFF\n"); //print to serial monitor that LED is OFF
    digitalWrite(LED_BUILTIN, LOW);// turn the LED off by making the voltage LOW
    delay(1000); // wait for a second
  }
}
