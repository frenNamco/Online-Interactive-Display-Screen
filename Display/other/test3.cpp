#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <toro_optimized.h>

#define BUTTON1      13
#define BUTTON2      12
#define BUTTON3      15
#define BUTTON4      17
#define BUTTON6      22
#define BUTTON5      21 

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite image = TFT_eSprite(&tft);
TFT_eSprite image2 = TFT_eSprite(&tft);
TFT_eSprite background = TFT_eSprite(&tft);
TFT_eSprite background2 = TFT_eSprite(&tft);

void setup() {
    Serial.begin(115200);

    pinMode(BUTTON1, INPUT_PULLUP);
    pinMode(BUTTON2, INPUT_PULLUP);
    pinMode(BUTTON3, INPUT_PULLUP);
    pinMode(BUTTON4, INPUT_PULLUP);
    pinMode(BUTTON5, INPUT_PULLUP);
    pinMode(BUTTON6, INPUT_PULLUP);

    tft.init();
    tft.setRotation(3);
    tft.setSwapBytes(true);
    tft.fillScreen(TFT_BLACK);
    
    background.createSprite(tft.width() / 2, tft.height());
    background2.createSprite(tft.width() / 2, tft.height());

    
    image.createSprite(53, 103);
    image.setSwapBytes(true);
    image2.createSprite(53, 103);
    image2.setSwapBytes(true);
}

int x = 0;
int y = 0;

void loop()
{
    // Push over existing background without clearing screen
    background.fillSprite(TFT_BLACK);
    background2.fillSprite(TFT_BLACK);
    image.pushImage(0, 0, 53, 103, toro_optimized);
    image.pushToSprite(&background, x, y, TFT_BLACK); // Transparent push (skip black pixels)
    image2.pushImage(0, 0, 53, 103, toro_optimized);
    image2.pushToSprite(&background2, x - (tft.width()/2), y, TFT_BLACK);

    
    background.pushSprite(0, 0);
    background2.pushSprite(tft.width() / 2, 0);


    if (digitalRead(BUTTON5) == LOW) {
        x+=2;
        if (x > tft.width()) x = -image.width();
    }  
    if (digitalRead(BUTTON4) == LOW) {
        x-=2;
        if (x < -image.width()) x = tft.width();
    } 
    if (digitalRead(BUTTON3) == LOW) {
        y-=2;
        if (y < -image.height()) y = tft.height();
    } 
    if (digitalRead(BUTTON6) == LOW) {
        y+=2;
        if (y > tft.height()) y = -image.height();
    }
    
}
