#include <Servo.h>

#define LED_PIN 4
#define SERVO_BACKEND_PIN 7
#define SERVO_FRONT_PIN 8

Servo servoFront, servoBack;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  servoFront.attach(SERVO_FRONT_PIN);
  servoBack.attach(SERVO_BACKEND_PIN);
  servoFront.write(90);
  servoBack.write(90);
  delay(2000);
}

void loop() {
  servoFront.write(70);
  delay(200);
  servoBack.write(70);
  delay(200);
  servoFront.write(110);
  delay(200);
  servoBack.write(110);
  delay(200);
}
