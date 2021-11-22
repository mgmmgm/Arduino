#include <Servo.h>
#include <SoftwareSerial.h>

//mode
#define INPUT_PIN 3
#define LED_PIN 4

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
volatile boolean shouldChangeState = false;

void setup() {
  pinMode(LED_PIN, OUTPUT);
  pinMode(INPUT_PIN, INPUT_PULLUP);
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
  stopCar();
  digitalWrite(LEFT_WHEEL_1, HIGH);
  digitalWrite(RIGHT_WHEEL_1, HIGH);
  analogWrite(LEFT_WHEEL_PWM, speed);
  analogWrite(RIGHT_WHEEL_PWM, speed);  
}

void moveBackward(int speed) {
  stopCar();
  digitalWrite(LEFT_WHEEL_2, HIGH);
  digitalWrite(RIGHT_WHEEL_2, HIGH);
  analogWrite(LEFT_WHEEL_PWM, speed);
  analogWrite(RIGHT_WHEEL_PWM, speed);  
}

void turnRight(int speed) {
  stopCar();
  digitalWrite(LEFT_WHEEL_1, HIGH);
  digitalWrite(RIGHT_WHEEL_2, HIGH);
  analogWrite(LEFT_WHEEL_PWM, speed);
  analogWrite(RIGHT_WHEEL_PWM, speed);  
}

void turnLeft(int speed) {
  stopCar();
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
  int time = random(500, 1000);
  delay(time);
  stopCar();
  rotateServo(90);  
  delay(2000);
}

void rotateServo(int ang) {
  servoFront.write(ang);
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
  if (shouldChangeState) {
    digitalWrite(LED_PIN, HIGH);
    delay(2000);
    shouldChangeState = false;
    digitalWrite(LED_PIN, LOW);
  }
  if (SerialBT.available()) { 
    msg = SerialBT.readString();
    if (msg == "1") {
      digitalWrite(LED_PIN, HIGH);
      SerialBT.println("Led is On");
    } else if (msg == "0") {
      digitalWrite(LED_PIN, LOW);
      SerialBT.println("Led is Off");
    } else {
      SerialBT.println(msg);
    }
 }
//  myDistance = radarPing();
//  if (myDistance > 50) {
//    mySpeed = min(mySpeed + 5, 255);
//    moveForward(mySpeed);
//    rotateServo(90);
//  } else {
//    moveBackward(250);
//    delay(1000);
//    stopCar();
//    mySpeed = 130;
//    turn();      
//  }
//  delay(100);
}


void onBtnChange() {
  shouldChangeState = true;
}
