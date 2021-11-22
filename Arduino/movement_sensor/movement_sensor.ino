#define SENSOR_PIN 7
#define LED_PIN 6

void setup() {
  pinMode(SENSOR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);

}

void loop() {
  int result = digitalRead(SENSOR_PIN);
  if (result == HIGH) {
    digitalWrite(LED_PIN, result);   
  } else {
    digitalWrite(LED_PIN, LOW);
  }
  delay(1000);
}
