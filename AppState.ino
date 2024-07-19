#include <EEPROM.h>

#include "AppState.hpp"

//persitsing & debouncing
const int EEPROMAddress = 0;  // EEPROM address to store the current exposure time index

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
  // Load the last saved exposure time index from EEPROM
  exposureIndex = EEPROM.read(EEPROMAddress);
  if (exposureIndex < 0 || exposureIndex >= exposureCount) {
    exposureIndex = 0;  // Default to the first exposure time if the index is out of range
  }
  millisValue = millisFromExposure();
  InputUnit preferredInputUnit = EXPOSUREVALUE;
  millisValue = 0;      // Milliseconds value from the user input
  correctionValue = 0;  // Correction value in microseconds
  refreshCurrentDelayTime();
}

//updates the user prefs with the current value
void saveUserPrefs() {
  unsigned int eePromLength = EEPROM.length();  //Get the total number of bytes on the eeprom
  PRINT(eePromLength);
  Serial.println(eePromLength);
}

void refreshCurrentDelayTime() {
  currentDelayTime = millisValue * 1000 + correctionValue;
}
