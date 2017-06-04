int rand_pos = 10;
int shoot = 1;


void setup() {
  Serial.begin(115200);

  while (!Serial);
  //randomSeed(analogRead(1));

}

void loop() {
  //int rand_pos=random(730);
  //rand_pos +=10;

  if (rand_pos > 725) {
    rand_pos = 20;
    rand_pos=rand_pos +5;
  }
  else {
    rand_pos=rand_pos + 5;
  }

  Serial.println(rand_pos);
  delay(100);

  Serial.println(shoot);
  delay(50);



}
