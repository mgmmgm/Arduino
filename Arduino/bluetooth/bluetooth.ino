#include <SoftwareSerial.h>

const int BTRX = 0;  
const int BTTX = 1;  
SoftwareSerial SerialBT(BTRX, BTTX);
  
#define LED_PIN 7
char incomingByte;  // incoming data
String msg;

void setup()
{
  pinMode(LED_PIN, OUTPUT);
   SerialBT.begin(9600);
}

void loop() {
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
}
//  if (Serial.available() > 0) {  // if the data came
//    incomingByte = Serial.read(); // read byte
//    
//    if(incomingByte == '0') {
//       digitalWrite(LED_PIN, LOW);  // if 1, switch LED Off
//      
//    }
//    if(incomingByte == '1') {
//       digitalWrite(LED_PIN, HIGH); // if 0, switch LED on
//      
//    }
//  }
//}
