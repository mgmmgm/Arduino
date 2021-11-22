#include <Servo.h>
#include "IRLremote.h"

#define INPUT_PIN 3

// Servo
int servoPins[3] = {6, 7, 8};
Servo myServos[3];
int startAngles[3] = {110, 90, 100};
int curAngles[3];
int candleAngels[27] = {
  71,37,102,
  78,69,135,
  87,85,148,
  97, 109, 169,
  123, 104, 163,
  132,101,160,
  141,89,150,
  148,74,139,
  110,99,156 // SHAMASH
};

// IR
#define IR_PIN 2 // Infrared receiving pin
CNec IRLremote;
long irCodes[20] = {0x45, 0x47, 0x44, 0x40,0x43,0x7,0x15, 0x9, 0x16, 0x19, 0xD, 0xC, 0x18, 0x5E, 0x8, 0x1C, 0x5A, 0x42, 0x52, 0x4A};
String irBtns[20] = {"POWER", "MENU", "TEST","+", "PREVIOUS", "BACKWARD", "PLAY", "FORWARD", "0", "-", "C", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

//Sound
#define SPEAKER_PIN 10
#define C2  65.41
#define Db2 69.30
#define D2  73.42
#define Eb2 77.78
#define E2  82.41
#define F2  87.31
#define Gb2 92.50
#define G2  98.00
#define Ab2 103.83
#define LA2 110.00
#define Bb2 116.54
#define B2  123.47
#define C3  130.81
#define Db3 138.59
#define D3  146.83
#define Eb3 155.56
#define E3  164.81
#define F3  174.61
#define Gb3 185.00
#define G3  196.00
#define Ab3 207.65
#define LA3 220.00
#define Bb3 233.08
#define B3  246.94
#define C4  261.63
#define Db4 277.18
#define D4  293.66
#define Eb4 311.13
#define E4  329.63
#define F4  349.23
#define Gb4 369.99
#define G4  392.00
#define Ab4 415.30
#define LA4 440.00
#define Bb4 466.16
#define B4  493.88
#define C5  523.25
#define Db5 554.37
#define D5  587.33
#define Eb5 622.25
#define E5  659.26
#define F5  698.46
#define Gb5 739.99
#define G5  783.99
#define Ab5 830.61
#define LA5 880.00
#define Bb5 932.33
#define B5  987.77
#define C6  1046.50
#define Db6 1108.73
#define D6  1174.66
#define Eb6 1244.51
#define E6  1318.51
#define F6  1396.91
#define Gb6 1479.98
#define G6  1567.98
#define Ab6 1661.22
#define LA6 1760.00
#define Bb6 1864.66
#define B6  1975.53
#define C7  2093.00
#define Db7 2217.46
#define D7  2349.32
#define Eb7 2489.02
#define E7  2637.02
#define F7  2793.83
#define Gb7 2959.96
#define G7  3135.96
#define Ab7 3322.44
#define LA7 3520.01
#define Bb7 3729.31
#define B7  3951.07
#define C8  4186.01
#define Db8 4434.92
#define D8  4698.64
#define Eb8 4978.03
#define E 250
#define Q 2*E
#define QH Q+E
#define H 2*Q
#define W 2*H

volatile byte shouldAbort = false;

void setup() {
  Serial.begin(9600);
  pinMode(INPUT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INPUT_PIN), onAbort, FALLING);
  for (byte i=0; i < 3; i++) {
    myServos[i].attach(servoPins[i]);
  }
  initServos();
  logServos();
  IRLremote.begin(IR_PIN);
  delay(1000);
  logMsg("end setup");
}

void initServos() {
  for (byte i = 0; i < 3; i++) {
    myServos[i].write(startAngles[i]);
    curAngles[i] = startAngles[i];
  }  
}

void logServos() {
  for (byte i=0; i < 3; i++) {
    Serial.print(curAngles[i]);
    Serial.print(", ");
  }
  Serial.println("");
}

void logMsg(String msg) {
  Serial.print(msg);
  Serial.println("");
}

void onAbort() {
  shouldAbort = true;
  Serial.println("abort");
}

void moveServos(int targetAngles[]) {
  if (shouldAbort) {
    shouldAbort = false;
    return;
  }
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
    delay(50);
    moveServos(targetAngles);
    logServos();
  } else {
    logServos();
  }
}

void moveToStartPosition() {
  logMsg("moveToStartPosition");
  logServos();
  moveServos(startAngles);
  logMsg("end moveToStartPosition");
}

void moveUp() {
  int up[3] = {-1, -1, 102};
  moveServos(up);
}

void moveUp2() {
  moveUp();
  
  int up2[3] = {-1, 70, 100};
  moveServos(up2);
}

void goToCandle(int num) {
  int move1[3] = {candleAngels[(num-1)*3], -1, -1};
  moveServos(move1);
  int move2[3] = {-1, candleAngels[((num-1)*3)+1], candleAngels[((num-1)*3)+2]};
  moveServos(move2);
}

void lightHanukia(int num) {
  for (int i=num; i > 0; i--) {
    goToCandle(i);
    delay(1000);
    moveUp();
    if (shouldAbort) {
        shouldAbort = false;
        return;
      }
  }
  moveUp2();
  goToCandle(9);
  delay(1000);
  moveUp2();
  playMusic();
}

void loop() {
  if (IRLremote.available()) {
    auto data = IRLremote.read();
    String irBtn = getIrButton(data.command);
    if (irBtn == "POWER") {
      moveToStartPosition();
    } else if (irBtn == "MENU") {
      moveUp2();
    } else if (irBtn == "TEST") {
      playMusic();
    } else if (irBtn == "+") {
      int newAngles[3] = {-1, -1, curAngles[2]+1};
      moveServos(newAngles);
    } else if (irBtn == "PREVIOUS") {
      int newAngles[3] = {-1, curAngles[1]-1, -1};
      moveServos(newAngles);
    } else if (irBtn == "BACKWARD") {
      int newAngles[3] = {curAngles[0]+1, -1, -1};
      moveServos(newAngles);
    } else if (irBtn == "PLAY") {
      lightHanukia(3);
    } else if (irBtn == "FORWARD") {
      int newAngles[3] = {curAngles[0]-1, -1, -1};
      moveServos(newAngles);
    } else if (irBtn == "0") {
      moveToStartPosition();
    } else if (irBtn == "-") {
      int newAngles[3] = {-1, -1, curAngles[2]-1};
      moveServos(newAngles);
    } else if (irBtn == "C") {
      int newAngles[3] = {-1, curAngles[1]+1, -1};
      moveServos(newAngles);
    } else if (irBtn == "1") {
      goToCandle(1);
    } else if (irBtn == "2") {
      goToCandle(2);
    } else if (irBtn == "3") {
      goToCandle(3);
    } else if (irBtn == "4") {
      goToCandle(4);
    } else if (irBtn == "5") {
      goToCandle(5);
    } else if (irBtn == "6") {
      goToCandle(6);
    } else if (irBtn == "7") {
      goToCandle(7);
    } else if (irBtn == "8") {
      goToCandle(8);
    } else if (irBtn == "9") {
      goToCandle(9);
    }
        

  }
  delay(1000);
}

String getIrButton(unsigned long value) {
  Serial.println(value, HEX);
  for (int i=0; i < 20; i++) {
    if (irCodes[i] == value) {
      Serial.println(irBtns[i]);
      return irBtns[i];
    }
  }
  return "";
}

void playNote(int note, int duration) {
  tone(SPEAKER_PIN, note);
  delay(duration - 50);
  noTone(SPEAKER_PIN);
  delay(50);
}

void playMusic() {
  double notes[78] = {
    C5,G4,C5,F5,E5,D5,C5,G5,G5,LA5,D5,E5,F5,E5,D5,C5,
    C5,G4,C5,F5,E5,D5,C5,G5,G5,LA5,D5,E5,F5,E5,D5,C5,
    G5,G5,G5,LA5,B5,C6,G5,C6,B5,LA5,G5,G5,F5,E5,F5,D5,
    E5,F5,G5,LA5,D5,E5,F5,E5,C5,LA5,G5,F5,E5,F5,G5,
    E5,F5,G5,LA5,D5,E5,F5,E5,C5,LA5,G5,F5,E5,D5,C5};
  int durations[78] = {
    Q, Q, Q, Q, Q, Q,QH, E, Q,  Q, Q, E, E, Q, Q, H,
    Q, Q, Q, Q, Q, Q,QH, E, Q,  Q, Q, E, E, Q, Q,QH,
    E,QH, E,  Q, Q, H, H, Q, Q,  Q, Q, E, E, E, E, H,
    QH, E,QH,  E,QH, E, H, Q, Q,  Q, E, E, Q, Q, H,
    QH, E,QH,  E,QH, E, H, Q, Q,  Q, E, E, Q, Q, H};
  for (int i = 0; i < 78; i++) {
    if (shouldAbort) {
      noTone(SPEAKER_PIN);
      shouldAbort = false;
      return;
    } 
    playNote(notes[i], durations[i]);
  }
  noTone(SPEAKER_PIN);
}
