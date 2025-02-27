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
  preferredInputUnit = (InputUnit)EEPROM.read(EEPROMAddressInputMode);
  preferredInputUnit = (preferredInputUnit >= EXPOSUREVALUE && preferredInputUnit <= MILLISECONDS) ? preferredInputUnit : EXPOSUREVALUE;
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
  EEPROM.write(EEPROMAddressInputMode, preferredInputUnit);
}

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
  if (currentMode == EXPOSURE) {
    currentMode = CORRECTION;
  } else if (currentMode == CORRECTION || currentMode == PREFS) {
    currentMode = EXPOSURE;
  } else if (currentMode == SPLASH) {
    currentMode = EXPOSURE;
    updateInfo("Long £ for settings!", 0, 5);
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
  if (currentlyHighlightedPrefElement == 0) {
    selectedInputUnit = selectedInputUnit == EXPOSUREVALUE ? MILLISECONDS : EXPOSUREVALUE;
  } else if (currentlyHighlightedPrefElement == 1) {
    includeUserValues = !includeUserValues;
  } else if (currentlyHighlightedPrefElement == 2) {
    preferredInputUnit = selectedInputUnit;
    saveUserPrefs(includeUserValues);
    selectedInputUnit = preferredInputUnit;
    setEncoderToState();
    return true;
  }
  displayNeedsUpdate = true;
  delay(200);
  currentlyHighlightedPrefElement = 2;  // Highlight OK
  setEncoderToState();
  displayNeedsUpdate = true;
  return false;
}

// Info Update ==========================

// state
unsigned long infoShownAt = 0;  // the starting time of the current info shown, 0 if there should be no info switching
byte infoID;                    //the identifier of the currently shown information, only relevant if there are rules for the next information to show
byte infoDuration;

//Update the Info string, set , and set an optional display time after which the infoText must be removed or replaced
/*!
@brief  A simple text centered text output in the given box
@param newInfo: The text to display
@param identifier: an optional id to identify the content for follow up, 0 by default
@param displayDuration: The times in seconds to show the infom or zero if the info should be diplayed until something newcomes up. After the given time the next info is shown or the info is removed
*/
void updateInfo(const char* newInfo, byte identifier, byte displayDuration) {

    //Skip if current info is an Error, Errors can not be replaced, but may run out if the timer is set
  if (infoID == ERROR_INFO_ID) {
    return;
  }

  free((void*)info);  // If info was dynamically allocated before, free it

  // Allocate memory and copy newInfo to info
  info = strdup(newInfo);  // strdup allocates memory and copies the string
  if (info == NULL) {
    fprintf(stderr, "Memory allocation failed\n");
    exit(1);
  }
  displayNeedsUpdate = true;
  infoID = identifier;
  infoDuration = displayDuration;
  if (infoDuration != 0) {
    infoShownAt = millis();
  }
}

//Checks for automatic Info Updates, i.e. an informataion which should be hidden or replaced by a followup
void checkForInfoUpdate() {
  if (infoDuration == 0) { return; }
  unsigned long now = millis();
  unsigned long interval = now - infoShownAt;

  if (interval > (unsigned long)infoDuration * 1000) {
    if (infoID == 0) {
      updateInfo("");
    } else if (infoID == ERROR_INFO_ID) {
      infoID = 0;
      updateInfo("");
    } else if (infoID == PRIMARY_SPLASH_INFO_ID || infoID == TERTIARY_SPLASH_INFO_ID) {
      updateInfo(SECONDARY_SPLASH_INFO, SECONDARY_SPLASH_INFO_ID, 5);
    } else if (infoID == SECONDARY_SPLASH_INFO_ID) {
      updateInfo(TERTIARY_SPLASH_INFO, TERTIARY_SPLASH_INFO_ID, 2);
    }
  }
}
