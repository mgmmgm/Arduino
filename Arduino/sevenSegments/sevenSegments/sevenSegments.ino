
byte seven_seg_digits[10] = {
  B11111100,
  B01100000,
  B11011010,
  B11110010,
  B01100110,
  B10110110,
  B10111110,
  B11100000,
  B11111110,
  B11100110
};

int latchPin = 11;
int clockPin = 9;
int dataPin = 12;


void setup() {
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);

}

void sevenSegWrite(byte digit) {
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, LSBFIRST, seven_seg_digits[digit]);
  digitalWrite(latchPin, HIGH);
}

void loop() {

  for (int digit = 9; digit >= 0; --digit) {
    delay(1000);
    sevenSegWrite(digit);
  }

  delay(3000);

}
