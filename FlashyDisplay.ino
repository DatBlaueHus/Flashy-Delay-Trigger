#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "AppState.hpp"
#include "FlashyDisplay.hpp"

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//The box configuration of the boxes
const int nBoxes = 4;
const int boxes[nBoxes][4] = {
  { 0, 0, 66, 11 },
  { 66, 0, 66, 11 },
  { 0, 11, 132, 11 },
  { 0, 22, 132, 10 }
};

// triangle at the left of the box
void triangleLeft(int box[4], int t = 2) {
  display.fillTriangle(box[0], box[1] + box[3] / 2 - t,
                       box[0] + t, box[1] + box[3] / 2,
                       box[0], box[1] + box[3] / 2 + t, SSD1306_WHITE);
}

// triangle at the right of the box
void triangleRight(int box[4], int t = 2) {
  display.fillTriangle(box[0] + box[2] - t -1, box[1] + box[3] / 2,
                    box[0] + box[2]-1, box[1] + box[3] / 2 - t,
                    box[0] + box[2]-1, box[1] + box[3] / 2 + t, SSD1306_WHITE);
}

/*!
@brief  A simple text centered text output in the given box
@param  text: The text to display
@param iBox: The index of the box where to place the text
@param highlight: If true, highlight the box
@note   The text will wrap hardly even within word borders
*/
void displayText(String text, int iBox, bool highlight = false) {
  int* box = boxes[iBox];
  fillBox(box);
  if (highlight) {
    triangleLeft(box);
  }
  //Calculate offset
  int16_t* bounds = textBounds(text);

  // int16_t x1, y1, w, h;
  // display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);
  int offset[2] = { (box[2] - bounds[2]) / 2, (box[3] - bounds[3]) / 2 };
  printText(text, box, offset);
}

int16_t* textBounds(String text) {
  static int16_t arr[4];
  display.getTextBounds(text, 0, 0, arr, arr + 1, arr + 2, arr + 3);
  return arr;
}

void displayRadio(String text, int iBox, bool selected = false, bool highlight = false) {
  int* box = boxes[iBox];
  int offset[2] = { box[3] + 3, 2 };  //TODO: Magic Numbers >:-/
  fillBox(box);
  circle(box, selected);
  printText(text, box, offset);
  if (highlight) {
    invertBox(box);
  }
}

void displayCheckbox(String text, int iBox, bool selected = false, bool highlight = false) {
  int* box = boxes[iBox];
  int offset[2] = { box[3] + 3, 2 };  //TODO: Magic Numbers >:-/
  fillBox(box);
  square(box, selected);
  printText(text, box, offset);
  if (highlight) {
    invertBox(box);
  }
}

void displayButton(String text, int iBox, bool highlight = false) {
  displayText(text, iBox);
  int* box = boxes[iBox];
  if (highlight) {
    triangleLeft(box);
    triangleRight(box);
    invertBox(box);
  }
}

//inverts the box
void invertBox(int box[4]) {
  display.fillRect(box[0], box[1], box[2], box[3], SSD1306_INVERSE);
}

// fills the given box with background color
void fillBox(int box[4]) {
  display.fillRect(box[0], box[1], box[2], box[3], SSD1306_BLACK);
}

//prints the text with the given offset in the box
void printText(String text, int box[4], int offset[2]) {
  display.setCursor(box[0] + offset[0], box[1] + offset[1]);
  display.print(text);
}

void circle(int box[4], bool filled) {
  int c = box[3] / 2;
  int r = c - 1;  //TODO: Magic Numbers >:-/
  display.drawCircle(box[0] + c, box[1] + c, r, SSD1306_WHITE);
  if (filled) {
    display.fillCircle(box[0] + c, box[1] + c, r - 2, SSD1306_WHITE);  //TODO: Magic Numbers >:-/
  }
}

void square(int box[4], bool filled) {
  int o = 1;  //TODO: Magic Numbers >:-/
  int d = box[3] - 2 * o;

  display.drawRect(box[0] + o, box[1] + o, d, d, SSD1306_WHITE);
  if (filled) {
    display.fillRect(box[0] + o + 2, box[1] + o + 2, d - 4, d - 4, SSD1306_WHITE);
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
    displayText(String(millisValue) + "ms", 0, currentMode == EXPOSURE);
  } else {
    displayText(formatExposureTime(exposureIndex), 0, currentMode == EXPOSURE);
  }
  displayText(microsAsMillis(correctionValue, 1), 1, currentMode == CORRECTION);
  displayText(currentDelayTime < 0 ? "Can not time travel back in time" : microsAsMillis(currentDelayTime, 1), 2);
  display.display();
  delay(10);
}

// Shows the splash screen
void prefsScreen() {
  display.clearDisplay();
  displayRadio("Exposure", 0, selectedInputUnit == EXPOSURE, currentlyHighlightedPrefElement == 0);
  displayRadio("ms", 1, selectedInputUnit == MILLISECONDS, currentlyHighlightedPrefElement == 1);  //
  displayCheckbox("Save preset values", 2, includeUserValues, currentlyHighlightedPrefElement == 2);
  displayButton("OK", 3, currentlyHighlightedPrefElement == 3);
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
#ifdef DEBUG_PRINT
    Serial.println(F("SSD1306 allocation failed"));
#endif
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
      display.stopscroll();
      prefsScreen();
    } else {
      display.stopscroll();
      settingsScreen();
    }
  }
}
