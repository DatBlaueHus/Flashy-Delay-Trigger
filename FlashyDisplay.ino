#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "AppState.hpp"
#include "FlashyDisplay.hpp"

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void baseDisplaySetup() {
  // Clear the buffer and display it once
  display.clearDisplay();
  display.display();
  
  // Add a delay to stabilize display, depending on your display you might be able to shorten it
  delay(1000);
  
  // Set display configurations
  display.cp437(true);         // Use full 256 char 'Code Page 437' font
  display.setTextSize(1);      // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE); // Draw white text
}

void splashScreen(String appName, String version) {
  int16_t  x1, y1;
  uint16_t w, h;
  display.getTextBounds(appName, 0, 0, &x1, &y1, &w, &h);
  int offsetName = (132 - w) / 2;
  display.getTextBounds(version, 0, 0, &x1, &y1, &w, &h);
  int offsetVersion = (132 - w) / 2;
  
  display.setCursor(offsetName-1, 5);     // Start at 5/5
  display.print(appName);
  display.setCursor(offsetName+1, 7);     // Offsete for effect
  display.print(appName);
  display.setCursor(offsetVersion, 24);     // Start at top-left corner
  display.println(version);
  display.display();
  // Add a short delay before starting the scroll
  delay(500);
  display.startscrollleft(0, 1);
}

void setupDisplay() {
 
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    PRINT(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  baseDisplaySetup();

  String appName = "Flashy Delay Trigger";
  String version = "Version 0.1.1b";
  splashScreen(appName, version);
}

//The box configuration of the boxes
static int box[3][4] = {{0,0,44,10},{44,0,44,10},{88,0,44,10}};
int nBoxes = 3;

/*!
@brief  A simple text output at 0,0
@param  text: The text to display
@param box: The number of the text box where to place the text
@param highlight: If true, highlight the box
@note   The text will wrap hardly even within word borders
*/
void displayText(String text, int iBox, bool highlight) {
  display.stopscroll();
  if (iBox <= 0 || iBox > nBoxes) { //Fallback, if boy is not set correctly
    display.clearDisplay();
    display.setCursor(0, 0);     // Start at top-left corner
    display.print(text);
    display.display();
    delay(10);
  }
  else {
    //Calculate the box
    int16_t  x1, y1;
    uint16_t w, h;
    display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    int i = iBox -1;
    int t = 2;
    int offseth = (box[i][2] - w) / 2;
    int offsetv = (box[i][3] - h) / 2;
    display.fillRect(box[i][0], box[i][1], box[i][2], box[i][3], SSD1306_BLACK);
    //display.strokeRect(box[i][0], box[i][1], box[i][2], box[i][3], SSD1306_WHITE);
    if (highlight) {
      display.fillTriangle(box[i][0], box[i][1]+box[i][3]/2 - t, 
                           box[i][0] + t, box[i][1]+box[i][3]/2,
                           box[i][0], box[i][1]+box[i][3]/2 + t, SSD1306_WHITE);
    }
    display.setCursor(box[i][0] + offseth, box[i][1] + offsetv);     // Start at top-left corner
    display.print(text);
  }
}

void printExposure() {
  String changed = millisValue != calculateExposureMicroseconds(exposureIndex)/1000 ? "\xFA":"";
  displayText(changed+formatExposureTime(exposureIndex), 1, currentMode == EXPOSURE);
}

void printCorrection() {
  displayText(formatCorrectionValue(correctionValue), 3, currentMode == CORRECTION);
}

void printMillis() {
  displayText(String(millisValue)+ "ms", 2, currentMode == MILLIS);
}

void updateSettingsDisplay() {
  display.clearDisplay();
  printExposure();
  printCorrection();
  printMillis();
  display.display();
  delay(10);
}


