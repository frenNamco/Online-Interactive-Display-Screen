#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h> 

#define TFT_CS      5   // Green
#define TFT_RST     4  // Blue
#define TFT_DC      16  // Purple
#define TFT_MOSI    23  // Orange
#define TFT_CLK     18  // Yellow

#define BUTTON1      13
#define BUTTON2      12
#define BUTTON3      15
#define BUTTON4      17
#define BUTTON6      22
#define BUTTON5      21  

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

class Heart {
    public:
        int radius;
        int xCenter;
        int yCenter;

        Heart() {
            this->radius = 1;
            this->xCenter = 0;
            this->yCenter = 0;
        }

        Heart(int radius, int xCenter, int yCenter) {
            this->radius = radius;
            this->xCenter = xCenter;
            this->yCenter = yCenter;
        }

        void draw() {
            tft.drawCircle(xCenter - radius, yCenter - radius, radius, ILI9341_WHITE);
            tft.drawCircle(xCenter + radius, yCenter - radius, radius, ILI9341_WHITE);
            tft.fillRect(xCenter - (2 * radius), yCenter - radius, (4 * radius) + 2, (4 * radius) + 2, ILI9341_BLACK);
            tft.drawLine(xCenter - (2 * radius), yCenter - radius, xCenter, yCenter + radius + 5, ILI9341_WHITE);
            tft.drawLine(xCenter + (2 * radius), yCenter - radius, xCenter, yCenter + radius + 5, ILI9341_WHITE);
        }

        void erase() {
            tft.fillRect(xCenter - (2 * radius), yCenter - (2 * radius), (4 * radius) + 2, (4 * radius) + 2, ILI9341_BLACK);
        }
};

Heart hearts[5];
int screenNum = 0;

void setup() {
    Serial.begin(115200);

    tft.begin();
    tft.setRotation(3);
    tft.setTextWrap(true);

    pinMode(BUTTON1, INPUT_PULLUP);
    pinMode(BUTTON2, INPUT_PULLUP);
    pinMode(BUTTON3, INPUT_PULLUP);
    pinMode(BUTTON4, INPUT_PULLUP);
    pinMode(BUTTON5, INPUT_PULLUP);
    pinMode(BUTTON6, INPUT_PULLUP);

    Serial.println("LCD should work");
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.setCursor(0, 0);
    tft.fillScreen(ILI9341_BLACK);

    randomSeed(analogRead(5));

}

void loop() {
    if (digitalRead(BUTTON5) == LOW && screenNum < 4) {
        screenNum++;
        tft.fillScreen(ILI9341_BLACK);
        delay(200);
        
    }

    if (digitalRead(BUTTON4) == LOW && screenNum > 0) {
        screenNum--;
        tft.fillScreen(ILI9341_BLACK);
        delay(200);
    }

    if (screenNum == 0) {
        tft.setCursor(0, 0);
        tft.print("I started this project \nlike forever ago basically\nso it could be something \nfor you");
        
    } else if (screenNum == 1) {
        tft.setCursor(0,0);
        tft.print("What I planned for this \nstill isn't done but \nregardless I still wanted \nit to be something for youright now");
        tft.println("\n\nThere's still a lot I haveto do so it could be the \ngift I want to give you \nand I'm slowly making \nprogress there");
    } else if (screenNum == 2) {
        tft.setCursor(0, 0);
        tft.print("I know I've really hurt \nyou in the past week and \nI feel awful about that \nso I made this while you \nslept so I could show you \nthat I still love you \na lot");
        tft.println("\n\nI don't think it's that \nimpressive because \ndoing something like this was very new to me \nbut I hope you still like it");
    } else if (screenNum == 3) {
        tft.setCursor(0, 0);
        tft.print("A small animation will \nplay after this,\nI hope you enjoy");
    }

    if (screenNum == 4) {
        tft.setCursor(0, 0);
        tft.print("I love you so much, \nEllie Greer Bacon");

        for(int i = 0; i < 5; i++) {
            hearts[i].radius = 1;
            hearts[i].xCenter = random(20, 300);
            hearts[i].yCenter = random(50, 200);
        }

        for(int i = 1; i < 20; i++) {
            for(int j = 0; j < 5; j++) {
                hearts[j].radius = i;
                hearts[j].draw();
                delay(50);
            }

            for(int j = 0; j < 5; j++) {
                hearts[j].erase();
                delay(50);
            }
        }
    }
}