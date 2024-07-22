#include <EEPROM.h>

#include "AppState.hpp"

void refreshCurrentDelayTime() {
  currentDelayTime = max(millisValue * 1000 + correctionValue, 0);
}

//This sets up Serial only in Debug mode. It's a noop in builds that haven't define DEBUG_PRINT
void setupSerialForDebug() {
#ifdef DEBUG_PRINT
  if (Serial.availableForWrite()) {
    Serial.begin(57600);
    while (!Serial)
      ;
    delay(100);
  }
#endif
}

//persitsing & debouncing
const int EEPROMAddressMillis = 10;      // 4 bytes for long millis
const int EEPROMAddressCorrection = 14;  // 4 bytes for long correction
const int EEPROMAddressInputMode = 18;   // 1 byte for byte inputMode

//initializes app state from user prefs, if set
void setupLoadUserPrefs() {
  EEPROM.begin();
  // Load the last saved values from EEPROM
  EEPROM.get(EEPROMAddressMillis, millisValue);
  if (millisValue > 60000) { millisValue = 42; }
  EEPROM.get(EEPROMAddressCorrection, correctionValue);
  EEPROM.get(EEPROMAddressInputMode, preferredInputUnit);
  preferredInputUnit = (preferredInputUnit >= EXPOSUREVALUE && preferredInputUnit < MILLISECONDS) ? preferredInputUnit : EXPOSUREVALUE;
  selectedInputUnit = preferredInputUnit;
  exposureIndex = findNearestExposureIndex(millisValue);
  refreshCurrentDelayTime();
}

//updates the user prefs with the current value
void saveUserPrefs(bool includeUserValues) {
  EEPROM.begin();
  if (includeUserValues) {
    EEPROM.put(EEPROMAddressMillis, millisValue);
    EEPROM.put(EEPROMAddressCorrection, correctionValue);
  }
  EEPROM.put(EEPROMAddressInputMode, preferredInputUnit);
}

// state

//takes care that encoder is in line with the current input channel
void setEncoderToState() {
  if (encoderSetterXallback == NULL) { return; }
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
#ifdef DEBUG_PRINT
  Serial.println(F("switchMode"));
#endif
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
  currentlyHighlightedPrefElement = PREF_OK;
  currentMode = PREFS;
  setEncoderToState();
  displayNeedsUpdate = true;
}

//returns true if the dialog has handled the selection, or false if
bool closePrefDialog() {
#ifdef DEBUG_PRINT
  Serial.println("currentlyHighlightedPrefElement" + String(currentlyHighlightedPrefElement));
#endif
  if (currentlyHighlightedPrefElement == 0) {
    selectedInputUnit = selectedInputUnit == EXPOSUREVALUE ? MILLISECONDS : EXPOSUREVALUE;
  } else if (currentlyHighlightedPrefElement == 1) {
    includeUserValues = !includeUserValues;
  } else if (currentlyHighlightedPrefElement == 2) {
    preferredInputUnit = selectedInputUnit;
    saveUserPrefs(includeUserValues);
    return true;
  }
  displayNeedsUpdate = true;
  delay(200);
  currentlyHighlightedPrefElement = 2;  // Highlight OK
  setEncoderToState();
  displayNeedsUpdate = true;
  return false;
}

void updateInfo(String newInfo) {
  info = newInfo;
  displayNeedsUpdate = true;
}
