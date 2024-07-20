#include <EEPROM.h>

#include "AppState.hpp"

void refreshCurrentDelayTime() {
  currentDelayTime = millisValue * 1000 + correctionValue;
}

//This sets up Serial only in Debug mode. It's a noop in builds that haven't define DEBUG_PRINT
void setupSerialForDebug() {
#ifdef DEBUG_PRINT
  if (Serial.availableForWrite()) {
    Serial.begin(57600);
    while (!Serial)
      ;
    Serial.println("Serial initialized");
  }
#endif
}

//persitsing & debouncing
const int EEPROMAddressMillis = 10;      // 4 bytes for long millis
const int EEPROMAddressCorrection = 14;  // 4 bytes for long correction
const int EEPROMAddressInputMode = 18;   // 1 byte for byte inputMode

//initializes app state from user prefs, if set
void setupLoadUserPrefs() {
  // Load the last saved values from EEPROM
  EEPROM.get(EEPROMAddressMillis, millisValue);
  PRINT("Millis read: " + String(millisValue));
  if (millisValue < 0) { millisValue = 0; }
  PRINT("Millis corrected: " + String(millisValue));

  EEPROM.get(EEPROMAddressCorrection, correctionValue);
  PRINT("Correction read: " + String());

  byte temp;
  EEPROM.get(EEPROMAddressInputMode, temp);
  preferredInputUnit = (temp >= 0 && temp < 2) ? temp : EXPOSUREVALUE;
  PRINT("temp: " + String(temp) + " > "+ String(preferredInputUnit));
  exposureIndex = findNearestExposureIndex(millis);
  correctionValue = -200; //right now EEProm readout is unbounded and often wrong
  refreshCurrentDelayTime();
  setEncoderToState();
}

//updates the user prefs with the current value
void saveUserPrefs(bool includeUserValues) {
  if (includeUserValues) {
    PRINT("Millis written: " + String(millisValue));
    EEPROM.put(millisValue, EEPROMAddressMillis);
//    writeLongToEEProm(EEPROMAddressMillis, millisValue);
    PRINT("Correction written: " + String(correctionValue));
    EEPROM.put(correctionValue, EEPROMAddressCorrection);
//    writeLongToEEProm(EEPROMAddressCorrection, correctionValue);
  }
  PRINT("preferredInputUnit written: " + String(preferredInputUnit));
      EEPROM.put(preferredInputUnit, EEPROMAddressInputMode);
}

// state

//takes care that encoder is in line with the current input channel
void setEncoderToState() {
  if (currentMode == CORRECTION) {
    encoderSetterXallback(correctionValue / 100);
  } else if (currentMode == EXPOSURE) {
    if (preferredInputUnit == MILLISECONDS) {
      encoderSetterXallback(millisValue);
    } else {
      exposureIndex = exposureFromMillis();  // set encoder Value nearest to milli
      encoderSetterXallback(exposureIndex);
    }
  } else if (currentMode == PREFS) {
    encoderSetterXallback(currentlyHighlightedPrefElement);
  }
}

//switches to next screen
void switchToNextMode() {
  PRINT("switchToNextMode");
  if (currentMode == EXPOSURE) {
    currentMode = CORRECTION;
  } else if (currentMode == CORRECTION || currentMode == PREFS || currentMode == SPLASH) {
    currentMode = EXPOSURE;
  }
  setEncoderToState();
  displayNeedsUpdate = true;
}

// Prefernce Dialog ===================================================================

void openPrefsDialog() {
    selectedInputUnit = preferredInputUnit;
    includeUserValues = true;
    currentlyHighlightedPrefElement = prefDefaultSelection;
    currentMode = PREFS;
    setEncoderToState();
    displayNeedsUpdate = true;
}

//returns true if the dialog has handled the selection, or false if 
bool closePrefDialog() {
  PRINT("currentlyHighlightedPrefElement"+String(currentlyHighlightedPrefElement));
  if (currentlyHighlightedPrefElement == 0 || currentlyHighlightedPrefElement == 1) {
    selectedInputUnit = selectedInputUnit == EXPOSURE ? MILLISECONDS : EXPOSURE;
  } else if (currentlyHighlightedPrefElement == 2) {
    includeUserValues = !includeUserValues;
  } else if (currentlyHighlightedPrefElement == 3) {
    preferredInputUnit = selectedInputUnit;
    saveUserPrefs(includeUserValues);
    return true;
  }
  currentlyHighlightedPrefElement = 3; // Switch to OK
  displayNeedsUpdate = true;
  return false;
}