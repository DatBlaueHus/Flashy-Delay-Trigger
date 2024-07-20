#include <EEPROM.h>

#include "AppState.hpp"

//persitsing & debouncing
const int EEPROMAddressMillis = 0;  // 4 bytes for long millis
const int EEPROMAddressCorrection = 4;  // 4 bytes for long correction
const int EEPROMAddressInputMode = 8;  // 1 byte for byte inputMode

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

//initializes app state from user prefs, if set
void setupLoadUserPrefs() {
  // Load the last saved values from EEPROM
  millisValue = EEPROM.read(EEPROMAddressMillis);
  if (millisValue < 0) { millisValue = 0; }
  correctionValue = EEPROM.read(EEPROMAddressCorrection);
  byte temp = EEPROM.read(EEPROMAddressInputMode);
  preferredInputUnit = (temp >= 0 && temp < 2) ? temp : EXPOSUREVALUE;
  exposureIndex = findNearestExposureIndex(millis);
  refreshCurrentDelayTime();
}

//updates the user prefs with the current value
void saveUserPrefs() {

#ifdef DEBUG_PRINT
  unsigned int eePromLength = EEPROM.length();  //Get the total number of bytes on the eeprom
  PRINT(eePromLength);
#endif
}

void refreshCurrentDelayTime() {
  currentDelayTime = millisValue * 1000 + correctionValue;
}
