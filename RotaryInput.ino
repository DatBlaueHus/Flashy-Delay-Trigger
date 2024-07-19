#include <RotaryEncoder.h>

#include "AppState.hpp"
#include "FlashyDisplay.hpp"

#include "Utilities.hpp"

//The encoder – not statically instantiated because the port should be injected
RotaryEncoder encoder = RotaryEncoder(ROTARYENCODER1, ROTARYENCODER2, RotaryEncoder::LatchMode::FOUR3);

const unsigned long longPressDelay = 500;
const unsigned long debounceDelay = 10;  // the debounce time in ms; increase if the output flickers
unsigned long lastDebounceTime = 0;       // the last time the output was toggled


void setupRotaryInput() {

  // Pullup the 3 ports
  pinMode(ROTARYENCODER1, INPUT_PULLUP);
  pinMode(ROTARYENCODER2, INPUT_PULLUP);
  pinMode(ROTARYENCODERSWITCH, INPUT_PULLUP);

  // Set the encoder position to the saved position
  encoder.setPosition(0);
}

// Interrupt service routine to handle encoder updates
void handleRotaryDelay() {
  encoder.tick();  // Update the encoder state
  handleSwitchPress();
  if (millis() - lastDebounceTime > debounceDelay) {
    int newPos = encoder.getPosition();
    if (currentMode == EXPOSURE) {
      if (newPos < 0) {
        newPos = exposureCount - 1;
        encoder.setPosition(newPos);
      }
      if (preferredInputUnit == EXPOSUREVALUE) {
        handleExposureInput(newPos);
      } else {
        handleMillisInput(newPos);
      }
    } else if (currentMode == CORRECTION) {
      handleCorrectionInput(newPos);
    } else if (currentMode == PREFS) {
      handlePrefsInput(newPos);
    } else if (currentMode == SPLASH) {
      if (newPos != 0) {
        switchToNextMode();
      }
    }
    lastDebounceTime = millis();
  }
}

void handlePrefsInput(int newPos) {
  // not yet implemented
}

void handleExposureInput(int newPos) {
  if (newPos >= exposureCount) {
    newPos = 0;
    encoder.setPosition(newPos);
  }
  if (newPos != exposureIndex) {
    exposureIndex = newPos;
    millisValue = millisFromExposure();
    refreshCurrentDelayTime();
    displayNeedsUpdate = true;
  }
}

void handleMillisInput(int newPos) {
  if (newPos < 0) {
    newPos = 0;
    encoder.setPosition(newPos);
  }
  if (millisValue != newPos) {
    millisValue = newPos;
    exposureIndex = findNearestExposureIndex(millisValue);
    refreshCurrentDelayTime();
    displayNeedsUpdate = true;
  }
}

void handleCorrectionInput(int newPos) {
  int newCorrection = newPos * 100;
  if (correctionValue != newCorrection) {
    correctionValue = newCorrection;
    refreshCurrentDelayTime();
    displayNeedsUpdate = true;
  }
}

void handleSwitchPress() {
  static unsigned long lastSwitchTime = 0;
  static bool previousSwitchState = false;
  bool switchState = !digitalRead(ROTARYENCODERSWITCH);
  if (previousSwitchState != switchState) {  //State changed
    if (switchState) {                       //Übergang nach On
      lastSwitchTime = millis();
      handleShortPress();
      previousSwitchState = switchState;
      delay(20);
    } else {  //Übergang nach Off
      lastSwitchTime = 0;
      previousSwitchState = switchState;
      delay(20);
    }
  } else if (switchState) {
    if (lastSwitchTime != 0) {
      if (millis() - lastSwitchTime > longPressDelay) {  //Button is still pressed, and the long press delay passed
        handleLongPress();
        lastSwitchTime = 0;  //we have handled this
      }
    }
  }
}

void handleShortPress() {
  if (currentMode == EXPOSURE || currentMode == CORRECTION) {
    switchToNextMode();
  }
  if (currentMode == PREFS) {

  }
  if (currentMode == SPLASH) {
    currentMode = EXPOSURE;
    displayNeedsUpdate = true;
  }
}

void handleLongPress() {
  if (currentMode != PREFS) {
    encoder.setPosition(0);
    currentMode = PREFS;
    displayNeedsUpdate = true;
  }
  else {
    saveUserPrefs();
    switchToNextMode();
  }
}

void switchToNextMode() {
  PRINT("switchToNextMode");
  if (currentMode == EXPOSURE) {
    currentMode = CORRECTION;
    encoder.setPosition(correctionValue / 100);
  } else if (currentMode == CORRECTION || currentMode == PREFS || currentMode == SPLASH) {
    currentMode = EXPOSURE;
    if (preferredInputUnit == MILLISECONDS) {
      encoder.setPosition(millisValue);
    } else {
      exposureIndex = exposureFromMillis();  // set encoder Value nearest to milli
      encoder.setPosition(exposureIndex);
    }
  }
  displayNeedsUpdate = true;
}
