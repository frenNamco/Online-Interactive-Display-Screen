#include <Arduino.h>

const int outputPin = 25;
const int inputPin = 26;
const int ledPin = 2;

void setup() {
  pinMode(outputPin, OUTPUT);
  pinMode(inputPin, INPUT_PULLDOWN);  // Enable internal pull-down resistor
  pinMode(ledPin, OUTPUT);

  digitalWrite(outputPin, HIGH);

  Serial.begin(115200);
  Serial.println("Continuity tester started");
}

void loop() {
  int state = digitalRead(inputPin);

  if (state == HIGH) {
    Serial.println("✅ Continuity Detected!");
    digitalWrite(ledPin, HIGH);
  } else {
    Serial.println("❌ No Continuity.");
    digitalWrite(ledPin, LOW);
  }

  delay(250);
}
