#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "AppState.hpp"
#include "FlashyDisplay.hpp"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//The box configuration of the boxes
static int box[3][4] = {
  { 0, 0, 66, 10 },
  { 66, 0, 66, 10 },
  { 0, 12, 132, 20 }
};
int nBoxes = 3;

/*!
@brief  A simple text output at 0,0
@param  text: The text to display
@param box: The number of the text box where to place the text
@param highlight: If true, highlight the box
@note   The text will wrap hardly even within word borders
*/
void displayText(String text, int iBox, bool highlight = false) {
  display.stopscroll();
  if (iBox <= 0 || iBox > nBoxes) {  //Fallback, if boy is not set correctly
    display.clearDisplay();
    display.setCursor(0, 0);  // Start at top-left corner
    display.print(text);
    display.display();
    delay(10);
  } else {
    //Calculate the box
    int16_t x1, y1;
    uint16_t w, h;
    display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
    int i = iBox - 1;
    int t = 2;
    int offseth = (box[i][2] - w) / 2;
    int offsetv = (box[i][3] - h) / 2;
    display.fillRect(box[i][0], box[i][1], box[i][2], box[i][3], SSD1306_BLACK);
    //display.strokeRect(box[i][0], box[i][1], box[i][2], box[i][3], SSD1306_WHITE);
    if (highlight) {
      display.fillTriangle(box[i][0], box[i][1] + box[i][3] / 2 - t,
                           box[i][0] + t, box[i][1] + box[i][3] / 2,
                           box[i][0], box[i][1] + box[i][3] / 2 + t, SSD1306_WHITE);
    }
    display.setCursor(box[i][0] + offseth, box[i][1] + offsetv);  // Start at top-left corner
    display.print(text);
  }
}

void splashScreen() {
  int16_t x1, y1;
  uint16_t w, h;
  display.getTextBounds(APPNAME, 0, 0, &x1, &y1, &w, &h);
  int offsetName = (132 - w) / 2;
  display.getTextBounds(VERSION, 0, 0, &x1, &y1, &w, &h);
  int offsetVersion = (132 - w) / 2;

  display.setCursor(offsetName - 1, 5);  // Start at 5/5
  display.print(APPNAME);
  display.setCursor(offsetName + 1, 7);  // Offsete for effect
  display.print(APPNAME);
  display.setCursor(offsetVersion, 24);  // Start at top-left corner
  display.println(VERSION);
  display.display();
  // Add a short delay before starting the scroll
  delay(500);
  display.startscrollleft(0, 1);
}

void settingsScreen() {
  display.clearDisplay();
  if (preferredInputUnit == MILLISECONDS) {
    displayText(String(millisValue) + "ms", 1, currentMode == EXPOSURE);
  } else {
    displayText(formatExposureTime(exposureIndex), 1, currentMode == EXPOSURE);
  }
  displayText(microsAsMillis(correctionValue, 1), 2, currentMode == CORRECTION);
  displayText(currentDelayTime < 0 ? "0!" : microsAsMillis(currentDelayTime,1), 3);

  display.display();
  delay(10);
}

// Shows the splash screen
void prefsScreen() {
  display.clearDisplay();
  display.print("Prefs not implemented. Long Press to close");
  display.display();
}

void baseDisplaySetup() {
  // Clear the buffer and display it once
  display.clearDisplay();
  display.display();

  delay(1000);  // Add a delay to stabilize display, depending on your display you might be able to shorten it
  // Set display configurations
  display.cp437(true);                  // Use full 256 char 'Code Page 437' font
  display.setTextSize(1);               // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);  // Draw white text
}

void setupDisplay() {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    PRINT(F("SSD1306 allocation failed"));
    for (;;)
      ;  // Don't proceed, loop forever
  }
  baseDisplaySetup();
  currentMode = SPLASH;
  displayNeedsUpdate = true;
}

void updateDisplay() {
  if (displayNeedsUpdate) {
    displayNeedsUpdate = false;
    if (currentMode == SPLASH) {
      splashScreen();
    } else if (currentMode == PREFS) {
      prefsScreen();
    } else {
      settingsScreen();
    }
  }
}
