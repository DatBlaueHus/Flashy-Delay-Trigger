#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "AppState.hpp"
#include "FlashyDisplay.hpp"
#include "ScreenGeometrics.hpp"


Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setupDisplay() {
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
#ifdef DEBUG_PRINT
    Serial.println(F("SSD1306 allocation failed"));
#endif
    for (;;);  // Don't proceed, loop forever
  }
  baseDisplaySetup();
  currentMode = SPLASH;
  displayNeedsUpdate = true;
}

void baseDisplaySetup() {
  // Clear the buffer and display it once
  display.clearDisplay();
  display.setRotation(ROTATE_SCREEN);
  display.display();
  delay(1000);  // Add a delay to stabilize display, depending on your display you might be able to shorten it
  // Set display configurations
  display.cp437(true);                  // Use full 256 char 'Code Page 437' font
  display.setTextSize(1);               // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);  // Draw white text
}

// triangle at the left of the box
void triangleLeft(const byte* box, int t = 2) {
  display.fillTriangle(box[0], box[1] + box[3] / 2 - t,
                       box[0] + t, box[1] + box[3] / 2,
                       box[0], box[1] + box[3] / 2 + t, SSD1306_WHITE);
}

// triangle at the right of the box
void triangleRight(const byte* box, int t = 2) {
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
void displayText(const String text, const byte* box, const bool highlight = false) {
  fillBox(box);
  if (highlight) {
    triangleLeft(box);
  }
  //Calculate offset
  uint16_t size[2];
  textSize(text, size);

  int offset[2] = { (int)((box[2] - size[0]) / 2), (int)((box[3] - size[1]) / 2) };
  printText(text, box, offset);
}

uint16_t* textSize(const String text, uint16_t* size) {
  static int16_t pos[2];
  display.getTextBounds(text, 0, 0, pos, pos + 1, size, size +1);
  return size;
}

void displayRadio(const String text, const byte* box, bool selected = false, bool highlight = false) {
  int offset[2] = { box[3] + 3, 2 };  //TODO: Magic Numbers >:-/
  fillBox(box);
  circle(box, selected);
  printText(text, box, offset);
  if (highlight) {
    invertBox(box);
  }
}

void displayCheckbox(const String text, const byte* box, bool selected = false, bool highlight = false) {
  int offset[2] = { box[3] + 3, 2 };  //TODO: Magic Numbers >:-/
  fillBox(box);
  square(box, selected);
  printText(text, box, offset);
  if (highlight) {
    invertBox(box);
  }
}

void displayButton(const String text, const byte* box, bool highlight = false) {
  displayText(text, box);
  if (highlight) {
    triangleLeft(box);
    triangleRight(box);
    invertBox(box);
  }
}

//inverts the box
void invertBox(const byte* box) {
  display.fillRect(box[0], box[1], box[2], box[3], SSD1306_INVERSE);
}

// fills the given box with background color
void fillBox(const byte* box) {
  display.fillRect(box[0], box[1], box[2], box[3], SSD1306_BLACK);
}

//prints the text with the given offset in the box
void printText(String text, const byte* box, const int offset[2]) {
  display.setCursor(box[0] + offset[0], box[1] + offset[1]);
  display.print(text);
}

void circle(const byte* box, const bool filled) {
  int c = box[3] / 2;
  int r = c - 1;  //TODO: Magic Numbers >:-/
  display.drawCircle(box[0] + c, box[1] + c, r, SSD1306_WHITE);
  if (filled) {
    display.fillCircle(box[0] + c, box[1] + c, r - 2, SSD1306_WHITE);  //TODO: Magic Numbers >:-/
  }
}

void square(const byte* box, const bool filled) {
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
  if (ROTATE_SCREEN == 0) {
    display.startscrollleft(0, 1);
  }
  else {
    display.startscrollright(5, 7);
  }
}

void settingsScreen() {
  display.clearDisplay();
  if (preferredInputUnit == MILLISECONDS) {
    displayText(String(millisValue) + "ms", boxes[0], currentMode == EXPOSURE);
  } else {
    displayText(formatExposureTime(exposureIndex), boxes[0], currentMode == EXPOSURE);
  }
  displayText(microsAsMillis(correctionValue, 1), boxes[1], currentMode == CORRECTION);
  displayText(microsAsMillis(currentDelayTime, 1), boxes[2]);
  displayText(info, boxes[3]);
  display.display();
  delay(10);
}

// Shows the splash screen
void prefsScreen() {
  display.clearDisplay();
  displayRadio(F("Exposure"), boxes[0], selectedInputUnit == EXPOSUREVALUE, currentlyHighlightedPrefElement == 0);
  displayRadio(F("ms"), boxes[1], selectedInputUnit == MILLISECONDS, currentlyHighlightedPrefElement == 1);  //
  displayCheckbox(F("Save preset values"), boxes[2], includeUserValues, currentlyHighlightedPrefElement == 2);
  displayButton(F("OK"), boxes[3], currentlyHighlightedPrefElement == 3);
  display.display();
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
