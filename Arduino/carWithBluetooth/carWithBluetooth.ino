#include <Servo.h>
#include <SoftwareSerial.h> // for Bluetooth

//mode
#define INPUT_PIN 3
#define LED_PIN 4
#define BEEP_PIN 5 // sound

// motor
#define LEFT_WHEEL_PWM 6 //speed
#define LEFT_WHEEL_1 8 //forward
#define LEFT_WHEEL_2 7 //backward
#define RIGHT_WHEEL_PWM 11 // speed
#define RIGHT_WHEEL_1 13 // forward
#define RIGHT_WHEEL_2 12 // backward
int mySpeed = 130;

// servo
#define SERVO_FRONT_PIN 2
Servo servoFront;

// radar
#define TRIG_PIN 9
#define ECHO_PIN 10
int myDistance = 0;

// bluetooth
const int BTRX = 0;  
const int BTTX = 1;  
SoftwareSerial SerialBT(BTRX, BTTX);
String msg;

boolean ledState = false;
volatile boolean shouldChangeToManualMode = false;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(INPUT_PIN, INPUT_PULLUP);
  pinMode(BEEP_PIN, OUTPUT);
  pinMode(LEFT_WHEEL_PWM, OUTPUT);
  pinMode(LEFT_WHEEL_1, OUTPUT);
  pinMode(LEFT_WHEEL_2, OUTPUT);
  pinMode(RIGHT_WHEEL_PWM, OUTPUT);
  pinMode(RIGHT_WHEEL_1, OUTPUT);
  pinMode(RIGHT_WHEEL_2, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(INPUT_PIN), onBtnChange, FALLING);
  servoFront.attach(SERVO_FRONT_PIN);
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  SerialBT.begin(9600);

  stopCar();
  rotateServo(90);
  delay(5000);
}

void stopCar() {
  digitalWrite(LEFT_WHEEL_PWM, 0);
  digitalWrite(LEFT_WHEEL_1, LOW);
  digitalWrite(LEFT_WHEEL_2, LOW);
  digitalWrite(RIGHT_WHEEL_PWM, 0);
  digitalWrite(RIGHT_WHEEL_1, LOW);
  digitalWrite(RIGHT_WHEEL_2, LOW);  
}

void moveForward(int speed) {
  digitalWrite(LEFT_WHEEL_1, HIGH);
  digitalWrite(RIGHT_WHEEL_1, HIGH);
  analogWrite(LEFT_WHEEL_PWM, speed);
  analogWrite(RIGHT_WHEEL_PWM, speed);  
}

void moveBackward(int speed) {
  digitalWrite(LEFT_WHEEL_2, HIGH);
  digitalWrite(RIGHT_WHEEL_2, HIGH);
  analogWrite(LEFT_WHEEL_PWM, speed);
  analogWrite(RIGHT_WHEEL_PWM, speed);  
}

void turnRight(int speed) {
  digitalWrite(LEFT_WHEEL_1, HIGH);
  digitalWrite(RIGHT_WHEEL_2, HIGH);
  analogWrite(LEFT_WHEEL_PWM, speed);
  analogWrite(RIGHT_WHEEL_PWM, speed);  
}

void turnLeft(int speed) {
  digitalWrite(LEFT_WHEEL_2, HIGH);
  digitalWrite(RIGHT_WHEEL_1, HIGH);
  analogWrite(LEFT_WHEEL_PWM, speed);
  analogWrite(RIGHT_WHEEL_PWM, speed);  
}

void turn() {
  int lDistance = 0;
  int rDistance = 0;
  rotateServo(30);
  delay(1000);
  lDistance = radarPing();
  delay(100);
  rotateServo(150);
  delay(1000);
  rDistance = radarPing();
  delay(100);
  if (lDistance > rDistance) {
    turnLeft(255);       
  } else {
    turnRight(255); 
  }
  int time = random(300, 700);
  delay(time);
  stopCar();
  rotateServo(90);  
}

void rotateServo(int ang) {
  servoFront.write(ang);
}

void startBeep() {
  digitalWrite(BEEP_PIN, HIGH);
}

void stopBeep() {
  digitalWrite(BEEP_PIN, LOW);
}

int radarPing() {
  long duration;
  int distance;
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);
  duration = pulseIn(ECHO_PIN, HIGH);
  distance = duration*0.0343/2; 
  return distance;
}

void loop() {
  if (shouldChangeToManualMode) {
    digitalWrite(LED_PIN, HIGH);
    rotateServo(90);
    myDistance = radarPing();
    if (myDistance > 30) {
      manualDriveMode();
    } else {
      stopCar();
      moveBackward(180);
      startBeep();
      delay(2000);
      stopBeep();
      stopCar();
    }
  } else {
    digitalWrite(LED_PIN, LOW);
    autonomicDriveMode();
  }
}

void autonomicDriveMode() {
  myDistance = radarPing();
  if (myDistance > 30) {
    mySpeed = min(mySpeed + 5, 255);
    moveForward(mySpeed);
    rotateServo(90);
  } else {
    stopCar();
    moveBackward(180);
    startBeep();
    delay(1000);
    stopBeep();
    stopCar();
    mySpeed = 130;
    turn();
    delay(1000);  
  }
  delay(100);
}

void manualDriveMode() {
  if (SerialBT.available()) { 
    msg = SerialBT.readString();
    if (msg == "F") {
      moveForward(255);
      SerialBT.println("Go Forward");
    } else if (msg == "B") {
      stopCar();
      moveBackward(180);
      SerialBT.println("Go Backward");
    } else if (msg == "L") {
      turnLeft(180);
      SerialBT.println("Turn Left");
    } else if (msg == "R") {
      turnRight(180);
      SerialBT.println("Turn Right");
    } else if (msg == "S") {
      stopCar();
      SerialBT.println("Stop car");
    } else {
      SerialBT.println("No command: " + msg);
    }
 }
}

void onBtnChange() {
  shouldChangeToManualMode = !shouldChangeToManualMode;
}
