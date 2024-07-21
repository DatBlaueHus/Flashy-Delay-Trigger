#include <EEPROM.h>

#include "AppState.hpp"

void refreshCurrentDelayTime() {
  currentDelayTime = max(millisValue * 1000 + correctionValue,0);
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
#ifdef DEBUG_PRINT
  Serial.println("Millis in: " + String(millisValue));
#endif
  if (millisValue < 0) { millisValue = 0; }
#ifdef DEBUG_PRINT
  Serial.println("Millis corr: " + String(millisValue));
#endif
  EEPROM.get(EEPROMAddressCorrection, correctionValue);
#ifdef DEBUG_PRINT
  Serial.println("Corr. in: " + String());
#endif

  byte temp;
  EEPROM.get(EEPROMAddressInputMode, temp);
  preferredInputUnit = (temp >= 0 && temp < 2) ? temp : EXPOSUREVALUE;
#ifdef DEBUG_PRINT
  Serial.println("inp: " + String(temp) + " > " + String(preferredInputUnit));
#endif
  exposureIndex = findNearestExposureIndex(millis);
  correctionValue = -200;  //right now EEProm readout is unbounded and often wrong
  refreshCurrentDelayTime();
}

//updates the user prefs with the current value
void saveUserPrefs(bool includeUserValues) {
  EEPROM.begin();
  if (includeUserValues) {
    EEPROM.put(millisValue, EEPROMAddressMillis);
    EEPROM.put(correctionValue, EEPROMAddressCorrection);
  }
  EEPROM.put(preferredInputUnit, EEPROMAddressInputMode);
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
  currentlyHighlightedPrefElement = prefDefaultSelection;
  currentMode = PREFS;
  setEncoderToState();
  displayNeedsUpdate = true;
}

//returns true if the dialog has handled the selection, or false if
bool closePrefDialog() {
#ifdef DEBUG_PRINT
  Serial.println("currentlyHighlightedPrefElement" + String(currentlyHighlightedPrefElement));
#endif
  if (currentlyHighlightedPrefElement == 0 || currentlyHighlightedPrefElement == 1) {
    selectedInputUnit = selectedInputUnit == EXPOSURE ? MILLISECONDS : EXPOSURE;
  } else if (currentlyHighlightedPrefElement == 2) {
    includeUserValues = !includeUserValues;
  } else if (currentlyHighlightedPrefElement == 3) {
    preferredInputUnit = selectedInputUnit;
    saveUserPrefs(includeUserValues);
    return true;
  }
  currentlyHighlightedPrefElement = 3;  // Switch to OK
  displayNeedsUpdate = true;
  return false;
}

void updateInfo(String newInfo) {
  info = newInfo;
  displayNeedsUpdate = true;
}
