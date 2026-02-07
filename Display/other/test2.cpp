#include <Arduino.h>

#define BUTTON1      13
#define BUTTON2       12
#define BUTTON3       15
#define BUTTON4     17
#define BUTTON6    22
#define BUTTON5     21  

void setup() {
    Serial.begin(115200);

    pinMode(BUTTON1, INPUT_PULLUP);
    pinMode(BUTTON2, INPUT_PULLUP);
    pinMode(BUTTON3, INPUT_PULLUP);
    pinMode(BUTTON4, INPUT_PULLUP);
    pinMode(BUTTON5, INPUT_PULLUP);
    pinMode(BUTTON6, INPUT_PULLUP);
    
}

void loop() {
    if (digitalRead(BUTTON1) == LOW) {
        Serial.println("Button 1");
    }

    if (digitalRead(BUTTON2) == LOW) {
        Serial.println("Button 2");
    }

    if (digitalRead(BUTTON3) == LOW) {
        Serial.println("Button 3");
    }

    if (digitalRead(BUTTON4) == LOW) {
        Serial.println("Button 4");
    }

    if (digitalRead(BUTTON5) == LOW) {
        Serial.println("Button 5");
    }

    if (digitalRead(BUTTON6) == LOW) {
        Serial.println("Button 6");
    }

    
}