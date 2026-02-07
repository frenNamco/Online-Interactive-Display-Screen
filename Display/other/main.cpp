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

#define YES_BUTTON      13
#define NO_BUTTON       12
#define UP_BUTTON       15
#define DOWN_BUTTON     17
#define RIGHT_BUTTON    22
#define LEFT_BUTTON     21  

// WiFi credentials
String ssid = "";
String password = "";

// Web server
WebServer server(80);
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

// HTML content
const char* htmlContent = R"rawliteral(
<!DOCTYPE html>
<html>
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>The Freak Display</title>
        <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/bootstrap/5.3.0/css/bootstrap.min.css">
        <link href="https://fonts.googleapis.com/css2?family=Baloo+2&display=swap" rel="stylesheet">
        <style>
            body { font-family: 'Baloo 2', cursve; background-color: #0C003D; text-align: center;}
            h1 { color:#28002D; font-weight: bolder;}
            p { color: #BEAEFF}
        </style>
    </head>
    <body>
        <nav class="navbar navbar-light" style="background-color: #F2027E;">
            <span class="navbar-brand mb-0 p h1" style="padding-left: 10px;">The Girlfriend Display</span>
        </nav>
        
        <div style="background-color:#471147; padding-top: 50px; padding-bottom: 50px" class="container-fluid">
            <h1 style="color: #FE90C9">The Girlfriend Display</h1>
            <p>Send something to your girlfriend!!!!</p>
            <form action="/submit" method="get" style="margin-top: 20px;">
                <!-- <label for="name">Enter your name:</label> -->
                <input type="text" id="name" name="msg" required class="form-control mb-3" style="width: 300px; margin: auto;">
                <input type="submit" class="btn" value="Send" style="background-color: turquoise;">
            </form>
        </div>
    </body>
</html>
)rawliteral";

String msg;
bool msgReceived = false; 

// Function to handle form submission
void handleFormSubmission() {
    msg = server.arg("msg"); 
    msgReceived = true;

    server.send(200, "text/html", htmlContent);
}

// Serve the main HTML content
void handleRoot() {
    server.send(200, "text/html", htmlContent);
}

void setup() {
    Serial.begin(115200);

    pinMode(YES_BUTTON, INPUT_PULLUP);
    pinMode(NO_BUTTON, INPUT_PULLUP);

    tft.begin();
    tft.setRotation(3);
    tft.setTextWrap(true);

    Serial.println("LCD should work");
    tft.setTextColor(ILI9341_WHITE);
    tft.setTextSize(2);
    tft.setCursor(0, 0);
    tft.fillScreen(ILI9341_BLACK);
    tft.println("hello");

    bool connected = false;
    
    do {
        tft.setCursor(0, 0);
        tft.fillScreen(ILI9341_BLACK);  
        tft.println("Connect to computer and \nopen CoolTerm\n");
        tft.print("Input SSID: ");
        Serial.println("Input SSID");
        while (ssid == "") {
            if (Serial.available()) {  // Check if data is available
                String receivedData = Serial.readStringUntil('\n');  // Read until newline
                ssid = receivedData;
            }
        } 
        tft.println(ssid);

        tft.print("\nInput Password: ");
        Serial.println("Input Password");
        while (password == "") {
            
            if (Serial.available()) {  // Check if data is available
                String receivedData = Serial.readStringUntil('\n');  // Read until newline
                password = receivedData;
            }
        }
        tft.println(password);
        
        tft.println("\nIs this correct?");
        bool buttonPress = false;
        int yesButtonState, noButtonState;
        while (buttonPress == false) {
            yesButtonState = digitalRead(YES_BUTTON);
            noButtonState = digitalRead(NO_BUTTON);

            if (yesButtonState == LOW) {
                unsigned long firstTime = millis();
                unsigned long secondTime = millis();
                const long interval = 5000;
                
                tft.print("Connecting");
                WiFi.begin(ssid.c_str(), password.c_str());
                while ((secondTime - firstTime <= interval) && connected == false) {
        
                    if (WiFi.status() != WL_CONNECTED) {
                        delay(500);
                        Serial.print(".");
                        tft.print(".");
                    } else {
                        connected = true;
                        buttonPress = true;
                        Serial.println("\nWiFi connected. IP address: " + WiFi.localIP().toString());
                    
                        // Set up SPIFFS
                        if (!SPIFFS.begin(true)) {
                            Serial.println("SPIFFS initialization failed!");
                            return;
                        }
                    }

                    secondTime = millis();
                }
                
                if (secondTime - firstTime >= interval) {
                    tft.fillScreen(ILI9341_BLACK);
                    tft.setCursor(0,0);
                    tft.println("Failed to connect");
                    tft.println("Try again with same SSID \nand password?");
                }
            
                while (buttonPress == false) {
                    yesButtonState = digitalRead(YES_BUTTON);
                    noButtonState = digitalRead(NO_BUTTON);
        
                    if (yesButtonState == LOW) {
                        buttonPress = true;
                    } else if (noButtonState == LOW) {
                        buttonPress = true;
                        ssid = "";
                        password = "";
                    }
                }
            } else if (noButtonState == LOW) {
                buttonPress = true;
                ssid = "";
                password = "";
            }
        }

        Serial.println(connected);

    } while (connected == false);


    // Connect to WiFi
    
    tft.fillScreen(ILI9341_BLACK);
    tft.setCursor(0, 0);  
    tft.println("Access IP: ");
    tft.println(WiFi.localIP().toString());
    tft.println("Waitng for Message");

    // Define routes
    server.on("/", handleRoot);
    server.on("/submit", handleFormSubmission);

    // Start server
    server.begin();
    Serial.println("Web server started!");
}

void loop() {
    server.handleClient();

    if (msgReceived) {
        Serial.println(msg);
        msgReceived = false;

        tft.fillScreen(ILI9341_BLACK);
        tft.setCursor(0, 0);
        tft.print(msg);

    }
}
