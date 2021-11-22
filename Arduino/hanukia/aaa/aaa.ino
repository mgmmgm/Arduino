#include <Servo.h>

// Servo
byte servoPins[3] = {6, 7, 8};
Servo myServos[3];
byte startAngles[3] = {90, 90, 100};
byte curAngles[3];

volatile boolean shouldChangeToManualMode = false;

void setup() {

  for (byte i=0; i < 3; i++) {
    myServos[i].attach(servoPins[i]);
  }
  initServos();
  delay(2000);
}

void initServos() {
  for (byte i=0; i < 3; i++) {
    myServos[i].write(startAngles[i]);
    curAngles[i] = startAngles[i];
  }
}

void moveServos(targetAngles) {
  bool moved = false;
  for (byte i=0; i < 3; i++) {
    if ((targetAngles[i] != -1) && (targetAngles[i] != curAngles[i])) {
       if (targetAngles[i] > curAngles[i]) {
        myServos[i].write(++curAngles[i]);
       } else if (targetAngles[i] < curAngles[i]) {
        myServos[i].write(--curAngles[i]);
       }
       moved = true;
    }
  }
  if (moved) {
    delay(100);
    moveServos(targetAngles);
  }
}

void candle4() {
  //moveServos({80, 50, 120});
}

void candle5() {
  //moveServos({90, 50, 120});
}

void up() {
  
}

void loop() {
  candle5();
  delay(15000);
  up();
  delay(5000);
  candle4();
  delay(85000);

  
  initServos();
  delay(5000);

}
