#include <RotaryEncoder.h>

#include "AppState.hpp"
#include "FlashyDisplay.hpp"

#include "Utilities.hpp"

//The encoder – not statically instantiated because the port should be injected
RotaryEncoder encoder = RotaryEncoder(ROTARYENCODER1, ROTARYENCODER2, RotaryEncoder::LatchMode::FOUR3);

const unsigned long longPressDelay = 500;
const unsigned long debounceDelay = 10;  // the debounce time in ms; increase if the output flickers
unsigned long lastDebounceTime = 0;      // the last time the output was toggled


void setupRotaryInput() {

  // Pullup the 3 ports
  pinMode(ROTARYENCODER1, INPUT_PULLUP);
  pinMode(ROTARYENCODER2, INPUT_PULLUP);
  pinMode(ROTARYENCODERSWITCH, INPUT_PULLUP);
  
  encoderSetterXallback = &setEncoder; //callback for the AppState to handle updates

  setEncoderToState();
}

void setEncoder(int newPos) {
  encoder.setPosition(newPos);
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
  if (newPos < 0) {
    newPos = nPrefElements - 1;
    encoder.setPosition(newPos);
  }
  if (newPos != currentlyHighlightedPrefElement) {
    int corrected = newPos % nPrefElements;
    encoder.setPosition(corrected);
    if (currentlyHighlightedPrefElement != corrected) {
      currentlyHighlightedPrefElement = corrected;
      displayNeedsUpdate = true;
    }
  }
}

void handleExposureInput(int newPos) {
  if (newPos >= exposureCount) {
    newPos = 0;
    encoder.setPosition(newPos);
  }
  if (newPos != exposureIndex) {
    exposureIndex = newPos;
    millisValue = millisFromExposure();
    #ifdef DEBUG_PRINT
    Serial.println(millisValue);
    #endif

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
    #ifdef DEBUG_PRINT
    Serial.println(millisValue);
    #endif
    refreshCurrentDelayTime();
    displayNeedsUpdate = true;
  }
}

void handleCorrectionInput(int newPos) {
  int newCorrection = newPos * 100;
  if (correctionValue != newCorrection) {
    correctionValue = newCorrection;
    #ifdef DEBUG_PRINT
    Serial.println(correctionValue);
    #endif
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
    if (closePrefDialog()) {
      switchToNextMode();
    }
  }
  if (currentMode == SPLASH) {
    currentMode = EXPOSURE;
    displayNeedsUpdate = true;
  }
}

void handleLongPress() {
  if (currentMode == PREFS) {
    if (closePrefDialog()) {
      switchToNextMode();
    }
  }
  else {
    openPrefsDialog();
  }
}
