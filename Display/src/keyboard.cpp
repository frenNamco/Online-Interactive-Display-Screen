#include <Arduino.h>
#include <WiFi.h>
#include <WebServer.h>
#include <SPIFFS.h>
#include <TFT_eSPI.h>
#include <SPI.h>
#include <esp_system.h>
#include <keyboardSprite.h>
#include <vector>

using namespace std;

#define BUTTON1      13
#define BUTTON2      12
#define BUTTON3      15
#define BUTTON4      17
#define BUTTON6      22
#define BUTTON5      21 

TFT_eSPI tft = TFT_eSPI();
TFT_eSprite textSpace = TFT_eSprite(&tft);

class Keyboard{
    int upButton = 3;
    int downButton = 6;
    int leftButton = 4;
    int rightButton = 5;
    vector<vector<uint8_t>> lowerCaseKeys = {
        {0x60, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x30, 0x2D, 0x3D},
        {0x71, 0x77, 0x65, 0x72, 0x74, 0x79, 0x75, 0x69, 0x6F, 0x70, 0x5B, 0x5D, 0x5C},
        {0x61, 0x73, 0x64, 0x66, 0x67, 0x68, 0x6A, 0x6B, 0x6C, 0x3B, 0x27},
        {0x7A, 0x78, 0x63, 0x76, 0x62, 0x6E, 0x6D, 0x2C, 0x2E, 0x2F},
        {0x10, 0x15, 0x20, 0x08, 0x85}
    };
    vector<vector<uint8_t>> upperCaseKeys = {
        {0x7E, 0x21, 0x40, 0x23, 0x24, 0x25, 0x5E, 0x26, 0x2A, 0x28, 0x29, 0x5F, 0x2B},
        {0x51, 0x57, 0x45, 0x52, 0x54, 0x59, 0x55, 0x49, 0x4F, 0x50, 0x7B, 0x7D, 0x7C},
        {0x41, 0x53, 0x44, 0x46, 0x47, 0x48, 0x4A, 0x4B, 0x4C, 0x3A, 0x22},
        {0x5A, 0x58, 0x43, 0x56, 0x42, 0x4E, 0x4D, 0x3C, 0x3E, 0x3F},
        {0x10, 0x15, 0x20, 0x08, 0x85}
    };
    vector<vector<uint16_t>> keyXCords = {{32}, {32}, {52}, {59}, {1, 41, 81, 201, 262}}; 
    uint16_t yStartingPoint = 7;

    uint16_t regularKeyWidth = 18;
    uint16_t specialKeysWidths[5] = {38, 38, 118, 58, 58};
    uint16_t keyHeight = 18;
    
    public:
        TFT_eSprite keyboardObject = TFT_eSprite(&tft);
        TFT_eSprite selectedKey = TFT_eSprite(&tft);
        const int width = 320;
        const int height = 106;
        int currentRow = 0;
        int currentCol = 0;

        uint16_t currXCord = keyXCords[currentRow][currentCol];
        uint16_t currYCord = yStartingPoint + (20 * currentRow);

        bool inUse = false;

        Keyboard() {
            for (int i = 0; i < lowerCaseKeys.size() - 1; i++) {
                for (int j = 1; j < lowerCaseKeys[i].size(); j++) {
                    keyXCords[i].push_back(keyXCords[i][j - 1] + 20);
                }
            }
        }

        void drawSelectedKey() {
            if (currentRow == lowerCaseKeys.size() - 1) {
                selectedKey.createSprite(specialKeysWidths[currentCol], keyHeight);
                selectedKey.drawRect(0, 0, specialKeysWidths[currentCol], keyHeight, TFT_CYAN);
            } else {
                selectedKey.createSprite(regularKeyWidth, keyHeight);
                selectedKey.drawRect(0,0,regularKeyWidth, keyHeight,TFT_CYAN);
            }
            selectedKey.pushToSprite(&keyboardObject, currXCord, currYCord, TFT_BLACK);
        }

        void deleteSelectedKey() {
            if (currentRow == lowerCaseKeys.size() - 1) {
                selectedKey.drawRect(0, 0, specialKeysWidths[currentCol], keyHeight, TFT_BLACK);
            } else {  
                selectedKey.drawRect(0,0,regularKeyWidth,keyHeight,TFT_BLACK);
            }
            
            selectedKey.pushToSprite(&keyboardObject, currXCord, currYCord);
            keyboardObject.pushImage(0, 0, width, height, keyboardSprite);
            selectedKey.deleteSprite();
        }

        void openKeyboard() {
            inUse = true;

            keyboardObject.createSprite(width, height);
            keyboardObject.setSwapBytes(true);
            keyboardObject.pushImage(0, 0, width, height, keyboardSprite);

            drawSelectedKey();
            keyboardObject.pushSprite(0, tft.height() - height);

            delay(250);
        }

        void closeKeyboard() {
            inUse = false;

            tft.fillRect(0, tft.height() - height, width, height, TFT_BLACK);
            keyboardObject.deleteSprite();

            delay(250);
        }

        void moveAroundKey (int buttonPress) {
            deleteSelectedKey();
            keyboardObject.pushSprite(0, tft.height() - height);

            if (buttonPress == leftButton && currentCol == 0) {
                currentCol = lowerCaseKeys[currentRow].size() - 1;
            } else if (buttonPress == leftButton) {
                currentCol--;
            }

            if (buttonPress == rightButton && currentCol == lowerCaseKeys[currentRow].size() - 1) {
                currentCol = 0;
            } else if (buttonPress == rightButton) {
                currentCol++;
            }

            if (buttonPress == downButton && currentRow == lowerCaseKeys.size() - 1) {
                currentRow = 0;
            } else if (buttonPress == downButton) {
                currentRow++;
            }

            if (buttonPress == upButton && currentRow == 0) {
                currentRow = lowerCaseKeys.size() - 1;
            } else if (buttonPress == upButton) {
                currentRow--;
            }
            
            currXCord = keyXCords[currentRow][currentCol];
            currYCord = yStartingPoint + (20 * currentRow); 
            
            drawSelectedKey();
            keyboardObject.pushSprite(0, tft.height() - height);

            delay(250);
        }

        void pressKey() {
            Serial.println((char)lowerCaseKeys[currentRow][currentCol]);
            delay(250);
        }

        void printAllKeys() {
            for (int i = 0; i < lowerCaseKeys.size(); i++) {
                for (int j = 0; j < lowerCaseKeys[i].size(); j++) {
                    if (lowerCaseKeys[i][j] == 0x08) {
                        Serial.print("<-");
                    } else if (lowerCaseKeys[i][j] == 0x85) {
                        Serial.print("->");
                    } else if (lowerCaseKeys[i][j] == 0x10) {
                        Serial.print("Shift");
                    } else if (lowerCaseKeys[i][j] == 0x15) {
                        Serial.print("CapsLock");
                    } else if (lowerCaseKeys[i][j] == 0x20) {
                        Serial.print("Space");
                    } else {
                        Serial.print((char)lowerCaseKeys[i][j]);
                    }
                    
                    Serial.print(" ");
                }

                Serial.println(" ");
            }

            for (int i = 0; i < upperCaseKeys.size(); i++) {
                for (int j = 0; j < upperCaseKeys[i].size(); j++) {
                    if (upperCaseKeys[i][j] == 0x08) {
                        Serial.print("<-");
                    } else if (upperCaseKeys[i][j] == 0x85) {
                        Serial.print("->");
                    } else if (upperCaseKeys[i][j] == 0x10) {
                        Serial.print("Shift");
                    } else if (upperCaseKeys[i][j] == 0x15) {
                        Serial.print("CapsLock");
                    } else if (upperCaseKeys[i][j] == 0x20) {
                        Serial.print("Space");
                    } else {
                        Serial.print((char)upperCaseKeys[i][j]);
                    }
                    
                    Serial.print(" ");
                }

                Serial.println(" ");
            }
        }
};

Keyboard keyboard;

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
    tft.fillScreen(TFT_BLACK);


    // textSpace.createSprite(320, tft.height() - keyboard.height);
    // textSpace.fillSprite(TFT_BLUE);
    // textSpace.pushSprite(0, 0);

    

    keyboard.printAllKeys();
}

void loop() {
    if (!keyboard.inUse && digitalRead(BUTTON2) == LOW) {
        keyboard.openKeyboard();
    } else if (keyboard.inUse) {
        if (digitalRead(BUTTON1) == LOW) keyboard.pressKey();
        if (digitalRead(BUTTON2) == LOW) keyboard.closeKeyboard();
        if (digitalRead(BUTTON3) == LOW) keyboard.moveAroundKey(3);
        if (digitalRead(BUTTON4) == LOW) keyboard.moveAroundKey(4);
        if (digitalRead(BUTTON5) == LOW) keyboard.moveAroundKey(5);
        if (digitalRead(BUTTON6) == LOW) keyboard.moveAroundKey(6);
    }

    

    
}
