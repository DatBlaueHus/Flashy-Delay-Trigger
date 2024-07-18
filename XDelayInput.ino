#include <RotaryEncoder.h>
#include <EEPROM.h>

#include "AppState.hpp"
#include "FlashyDisplay.hpp"

//The encoder – not statically instantiated because the port should be injected
RotaryEncoder *encoder;

// port configuration
int switchPort;

// debounce
const unsigned long longPressDelay = 500;
const unsigned long debounceDelay = 100;    // the debounce time in ms; increase if the output flickers
unsigned long lastDebounceTime = 0;  // the last time the output was toggled


//persitsing & debouncing
const int EEPROMAddress = 0; // EEPROM address to store the current exposure time index

//Converters ==================================================================================================

long calculateExposureMicroseconds(int index) {
  return static_cast<long>(exposureTimes[index] * 1000000);
}

//calculates the milliseconds from the current exposure value
long millisFromExposure() {
  return calculateExposureMicroseconds(exposureIndex)/1000;
}

int exposureFromMillis() {
  return findNearestExposureIndex(millisValue);
}

int findNearestExposureIndex(long milliseconds) {
  long microseconds = milliseconds * 1000;
  int nearestIndex = 0;
  long smallestDifference = abs(calculateExposureMicroseconds(0) - microseconds);
  for (int i = 1; i < exposureCount; i++) {
    long difference = abs(calculateExposureMicroseconds(i) - microseconds);
    if (difference < smallestDifference) {
      smallestDifference = difference;
      nearestIndex = i;
    }
    else { 
      return nearestIndex; // as soon as the difference is bigger, we can bail out!
    }
    
  }
  return nearestIndex;
}

//DisplayHelpers ==================================================================================================


// Formatters ==================================================================================================

String formatExposureTime(int index) {
  float time = exposureTimes[index];
  if (time >= 1.0) {
    return String(time, 0) + "\"";
  } else {
    float denominator = 1.0 / time;
    int roundedDenominator = (int)(denominator + 0.5); // Properly round the denominator

    // Check if the rounded denominator is close enough to the actual denominator
    if (abs(denominator - roundedDenominator) > 0.01) {
      return "1/" + String(denominator, 1);
    } else {
      return "1/" + String(roundedDenominator);
    }
  }
}

//Displays microseconds as ms with one digit after the point
String formatCorrectionValue(int value) {
  return String(value / 1000.0, 1) + "ms";
}



void setupRotaryDelay(int rotaryPort1, int rotaryPort2, int rotarySwitchPort) {
  
  //Persist the switch port
  switchPort = rotarySwitchPort;

  // Pullup the 3 ports
  pinMode(rotaryPort1, INPUT_PULLUP);
  pinMode(rotaryPort2, INPUT_PULLUP);
  pinMode(switchPort, INPUT_PULLUP);

  // Setup a RotaryEncoder with 2 steps per latch for the 2 signal input pins:
  encoder = new RotaryEncoder(rotaryPort1, rotaryPort2, RotaryEncoder::LatchMode::FOUR3);

  // Load the last saved exposure time index from EEPROM
  exposureIndex = EEPROM.read(EEPROMAddress);
  if (exposureIndex < 0 || exposureIndex >= exposureCount) {
    exposureIndex = 0;  // Default to the first exposure time if the index is out of range
  }
  millisValue = millisFromExposure();

    // Set the encoder position to the saved position
  encoder->setPosition(exposureIndex);
}


// Interrupt service routine to handle encoder updates
void handleRotaryDelay() {
  encoder->tick();  // Update the encoder state
  handleSwitchPress();
  if (millis() - lastDebounceTime > debounceDelay) {
    int newPos = encoder->getPosition();
    if (currentMode == EXPOSURE) {
      handleExposureInput(newPos);
      } else if (currentMode == CORRECTION) {
        handleCorrectionInput(newPos);
      } else if (currentMode == MILLIS) {
        handleMillisInput(newPos);
      }
    lastDebounceTime = millis();
  }
}

void handleExposureInput(int newPos) {
  if (newPos < 0) {
    newPos = exposureCount - 1;
    encoder->setPosition(newPos);
  } else if (newPos >= exposureCount) {
    newPos = 0;
    encoder->setPosition(newPos);
  }
  if (newPos != exposureIndex) {
    exposureIndex = newPos;
    millisValue = millisFromExposure();
    updateSettingsDisplay();
  }
}

void handleMillisInput(int newPos) {
  if (newPos < 0) {
    newPos = 0;
    encoder->setPosition(newPos);
  }
  if (millisValue != newPos) {
    millisValue = newPos;
    exposureIndex = findNearestExposureIndex(millisValue);
    updateSettingsDisplay();
  }
}

void handleCorrectionInput(int newPos) {
  int newCorrection = newPos * 100;
  if (correctionValue != newCorrection) {
    correctionValue = newCorrection;
    updateSettingsDisplay();
  }
}

void handleSwitchPress() {
  
  static unsigned long lastSwitchTime = 0;
  static bool previousSwitchState = false;
  bool switchState = !digitalRead(switchPort);
  if (previousSwitchState != switchState) { //State changed
    if (switchState) { //Übergang nach On
      lastSwitchTime = millis();
      switchToNextMode();
      previousSwitchState = switchState;
      delay(20);
    }
    else { //Übergang nach Off
      lastSwitchTime = 0;
      previousSwitchState = switchState;
      delay(20);
    }
  }
  else if (switchState) {
    if (lastSwitchTime != 0) {
      if (millis() - lastSwitchTime > longPressDelay) { //Button is still pressed, and the long press delay passed
        handleLongPress();
        lastSwitchTime = 0; //we have handled this
      }
    }
  }
}

void handleLongPress() {
  //TODO: Move out, add correction and store the millis value!
    EEPROM.write(EEPROMAddress, exposureIndex);
}

void switchToNextMode() {
    if (currentMode == EXPOSURE) {
      currentMode = CORRECTION;
      encoder->setPosition(correctionValue / 100);
    } else if (currentMode == CORRECTION) {
      currentMode = MILLIS;
      encoder->setPosition(millisValue);
    } else {
      currentMode = EXPOSURE;
      exposureIndex = exposureFromMillis(); // set encoder Value nearest to milli
      encoder->setPosition(exposureIndex);
    }
    updateSettingsDisplay();
}