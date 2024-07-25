#include <U8g2lib.h>
#include <Wire.h>

#include "AppState.hpp"
#include "ScreenGeometrics.hpp"
#include "Utilities.hpp"

//Font
//bdfconv -f 1 -m "28-255" -n SwissFoto -o SwissFoto.hpp SwissFoto.bdf
#include "SwissFoto.hpp"

//Choose the driver according to memory requirements
//U8G2_SSD1306_128X64_NONAME_1_HW_I2C display(U8G2_R2, /* reset=*/U8X8_PIN_NONE);  //small buffer
U8G2_SSD1306_128X64_NONAME_2_HW_I2C display(U8G2_R2, /* reset=*/U8X8_PIN_NONE);  // big buffer
//U8G2_SSD1306_128X64_NONAME_F_HW_I2C display(U8G2_R2, /* reset=*/U8X8_PIN_NONE);  // Fullscreen

#define I2CCLOCKFREQUENCY 800000  //Reduce if the screen doesn't start

#define U8G2_BLACK 0
#define U8G2_WHITE 1
#define U8G2_INVERSE 2

void setupDisplay() {
  display.setBusClock(500000);
  display.begin();
  display.setFont(SwissFoto);
  currentMode = SPLASH;
  char version[] = VERSION;
  transformString(version);
  char formattedString[50];
  snprintf(formattedString, sizeof(formattedString), VERSIONTEMPLATE, version);
  updateInfo(formattedString, PRIMARY_SPLASH_INFO_ID, 3);
  displayNeedsUpdate = true;
}

/*!
@brief  A simple text centered text output in the given box
@param  text: The text to display
@param iBox: The index of the box where to place the text
@param highlight: If true, highlight the box
@note   The text will wrap hardly even within word borders
*/
void displayText(const char* text, const byte* box, const char* symbol = NULL) {
  if (symbol != NULL) {
    unsigned int offset[2] = { TEXTDEDAULTOFFSETX, TEXTDEDAULTOFFSETY };
    printText(symbol, box, offset);
  }
  unsigned int* offset = centerOffset(text, box);
  printText(text, box, offset);
}

//calculates the offset to center the text in the box
unsigned int* centerOffset(const char* text, const byte* box) {
  static unsigned int arr[2];
  arr[0] = (box[2] - display.getStrWidth(text)) / 2;
  arr[1] = TEXTDEDAULTOFFSETY;
  return arr;
}

// prints the text left aligned with standard offset
void printTextWithPrefix(const char* text, const char* prefix, const byte* box) {
  // Determine the lengths of the prefix and the original string
  size_t prefixLen = strlen(prefix);
  size_t testLen = strlen(text);

  // Allocate memory for the new string
  char* result = (char*)malloc(prefixLen + testLen + 1);  // +1 for the null terminator
  strcpy(result, prefix);
  strcat(result, text);
  static unsigned int offset[2] = { TEXTDEDAULTOFFSETX, TEXTDEDAULTOFFSETY };
  printText(result, box, offset);
  free(result);  //free the memory!
}

void displayRadio(const char* text, const byte* box, bool selected = false) {
  if (selected) {
    printTextWithPrefix(text, "¦", box);
  } else {
    printTextWithPrefix(text, "§", box);
  }
}

void displayCheckbox(const char* text, const byte* box, bool selected = false) {
  if (selected) {
    printTextWithPrefix(text, "¤", box);
  } else {
    printTextWithPrefix(text, "¥", box);
  }
}

void displayButton(const char* text, const byte* box) {
    unsigned int* offset = centerOffset(text, box);
    offset[1] = offset[1] - 2;
    printText(text, box, offset);
}

//inverts the box
void invertBox(const byte* box) {
  display.setDrawColor(U8G2_INVERSE);
  display.drawBox(box[0], box[1], box[2], box[3]);  //drawBox
  display.setDrawColor(U8G2_WHITE);
}

//prints the text with the given offset in the box
void printText(const char* text, const byte* box, const unsigned int* offset) {
  display.drawUTF8(box[0] + offset[0], box[1] + offset[1], text);
}

void splashScreen() {
  display.firstPage();
  int offset = (SCREEN_WIDTH - display.getStrWidth(APPNAME)) / 2;
  int versionOffset = (SCREEN_WIDTH - display.getStrWidth(info)) / 2;
  do {  //page looping
    display.setCursor(offset, 26);
    display.print(APPNAME);
    display.setCursor(versionOffset, 60);  // Start at top-left corner
    display.print(info);
  } while (display.nextPage());
}

//The main setting screen to operate in
void settingsScreen() {
  // strdup allocates memory!
  const char* exposure = preferredInputUnit == MILLISECONDS ? strdup(formatMilliseconds(millisValue).c_str()) :  strdup(formatExposureTime(exposureIndex).c_str());
  const char* correction = strdup(microsAsMillis(correctionValue, 1).c_str());
  const char* totalDelay = strdup(microsAsMillis(currentDelayTime, 1).c_str());

  display.firstPage();
  do {  //page looping
    displayText(exposure, boxes[0], currentMode == EXPOSURE ? "¢" : "£");
    displayText(correction, boxes[1], currentMode == CORRECTION ? "¢" : "£");
    displayText(totalDelay, boxes[2]);
    displayText(info, boxes[3]);
  } while (display.nextPage());
  // delay(10);

  free((void*)exposure);
  free((void*)correction);
  free((void*)totalDelay);
}

// Shows the preference screen
void prefsScreen() {
  display.firstPage();
  
  String presetDisplay;
  if (selectedInputUnit == EXPOSUREVALUE) {
      presetDisplay = "Preset " + formatSmallExposureTime(exposureIndex) + "ª";
  }
  else {
      presetDisplay = "Preset " + formatMilliseconds(millisValue) + "ª";
  }
  do {  //page looping
    displayRadio("¨", boxes[0], selectedInputUnit == EXPOSUREVALUE);
    displayRadio("ms", boxes[1], selectedInputUnit == MILLISECONDS);  //
    displayCheckbox(presetDisplay.c_str(), boxes[2], includeUserValues);
    if (currentlyHighlightedPrefElement == PREF_OK) {
        displayButton("«   OK   ¬", boxes[3]);
    }
    else {
        displayButton("«     OK     ¬", boxes[3]);
    }
    handlePrefHighlight();
  } while (display.nextPage());
}

void handlePrefHighlight() {
    //Handle Highlight
    if (currentlyHighlightedPrefElement == PREF_RADIOGROUP) {
        invertBox(boxes[0]);
        invertBox(boxes[1]);
    }
    else if (currentlyHighlightedPrefElement == PREF_SETDEFAULTS) {
        invertBox(boxes[2]);
    } else if (currentlyHighlightedPrefElement == PREF_OK) {
        invertBox(boxes[3]);
    }
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
