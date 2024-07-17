#include <RotaryEncoder.h>
#include <EEPROM.h>

#include "XDelay.hpp"
#include "FlashyDisplay.hpp"

//The encoder I do not statically 
RotaryEncoder *encoder;

// Define the fixed list of exposure times (in seconds) and their microsecond equivalents
const float exposureTimes[] = {1.0/320, 1.0/250, 1.0/200, 1.0/160, 1.0/125, 1.0/100, 1.0/80, 1.0/60, 1.0/50, 1.0/40, 1.0/30, 1.0/25, 1.0/20, 1.0/15, 1.0/13, 1.0/10, 1.0/8, 1.0/6, 1.0/5, 1.0/4, 1.0/3, 1.0/2.5, 1.0/2, 1.0/1.6, 1.0/1.3, 1.0, 2.0, 4.0, 8.0};
long calculateExposureMicroseconds(int index);
const int exposureCount = sizeof(exposureTimes) / sizeof(exposureTimes[0]);

int exposureIndex = 0;  // To keep track of the current exposure time
long millisValue = 0; // Milliseconds value from the user input
long correctionValue = 0; // Correction value in microseconds

// debounce
const unsigned long debounceDelay = 100;    // the debounce time in ms; increase if the output flickers
unsigned long lastDebounceTime = 0;  // the last time the output was toggled

//Stores a changed index in EXPOSURE mode. Set in changes in EXPOSURE mode, Reset to -1 in changes in MILLIS mode
static long userExposureIndex = -1;

// port configuration
int switchPort;

//persitsing & debouncing
const int EEPROMAddress = 0; // EEPROM address to store the current exposure time index

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

    // Set the encoder position to the saved position
  encoder->setPosition(exposureIndex);
  
  // Print the initial exposure time
  Serial.println("Initial exposure time: 1/" + String(1.0/exposureTimes[exposureIndex]));
  Serial.println("Equivalent time in microseconds: " + String(calculateExposureMicroseconds(exposureIndex)));
}


// Interrupt service routine to handle encoder updates
void handleRotaryDelay() {
  encoder->tick();  // Update the encoder state
  handleSwitchPress();

  if (millis() - lastDebounceTime > debounceDelay) {
    int newPos = encoder->getPosition();
    if (currentMode == EXPOSURE) {
        if (newPos < 0) {
          newPos = exposureCount - 1;
          encoder->setPosition(newPos);
        } else if (newPos >= exposureCount) {
          newPos = 0;
          encoder->setPosition(newPos);
        }
        if (newPos != exposureIndex && millis() - lastDebounceTime > debounceDelay) {
          exposureIndex = newPos;
          millisValue = millisFromExposure();
          printExposure();
//        EEPROM.write(EEPROMAddress, currentIndex);
        }
      } else if (currentMode == CORRECTION) {
        int newCorrection = newPos * 100;
        if (correctionValue != newCorrection) {
          correctionValue = newCorrection;
          printCorrection();
        }
      } else if (currentMode == MILLIS) {
        if (newPos < 0) {
          newPos = 0;
          encoder->setPosition(newPos);
        }
        if (millisValue != newPos) {
          millisValue = newPos;
          printMillis();
        }
      }
    lastDebounceTime = millis();
  }  
}


void handleSwitchPress() {
  
  static unsigned long lastSwitchTime = 0;
  static bool switchState = false;
  if (digitalRead(switchPort) == LOW && millis() - lastSwitchTime > 300) {
    lastSwitchTime = millis();
    // Change mode on switch press
    if (currentMode == EXPOSURE) {
      currentMode = CORRECTION;
      printCorrection();
      encoder->setPosition(correctionValue / 100);
    } else if (currentMode == CORRECTION) {
      currentMode = MILLIS;
      encoder->setPosition(millisValue);
      printMillis();
    } else {
      currentMode = EXPOSURE;
      printExposure();
      exposureIndex = exposureFromMillis(); // set encoder Value nearest to milli
      encoder->setPosition(exposureIndex);
    }
  }
}

//Converters

//calculates the milliseconds from the current exposure value
unsigned long millisFromExposure() {
  return calculateExposureMicroseconds(exposureIndex)/1000;
}

long calculateExposureMicroseconds(int index) {
  return static_cast<long>(exposureTimes[index] * 1000000);
}

int exposureFromMillis() {
  return findNearestExposureIndex(millisValue);
}

int findNearestExposureIndex(long milliseconds) {
  long microseconds = milliseconds * 1000;
  int nearestIndex = 0;
  long smallestDifference = abs(calculateExposureMicroseconds(0) - microseconds);

  Serial.println("Input milliseconds: " + String(milliseconds));
  Serial.println("Input microseconds: " + String(microseconds));
  Serial.println("Starting with index 0, difference: " + String(smallestDifference));

  for (int i = 1; i < exposureCount; i++) {
    long difference = abs(calculateExposureMicroseconds(i) - microseconds);
    Serial.println("Index " + String(i) + ", exposure time: " + String(calculateExposureMicroseconds(i)) + ", difference: " + String(difference));

    if (difference < smallestDifference) {
      smallestDifference = difference;
      nearestIndex = i;
      Serial.println("New nearest index: " + String(nearestIndex) + ", smallest difference: " + String(smallestDifference));
    }
  }

  return nearestIndex;
}




//DisplayHelpers

void printMillis() {
  displayText("Milliseconds: " + String(millisValue));
}

void printExposure() {
  String changed = millisValue != calculateExposureMicroseconds(exposureIndex)/1000 ? "~":"";
  displayText("Exp: "+changed+formatExposureTime(exposureIndex));
}

void printCorrection() {
  displayText("Correction "+formatCorrectionValue(correctionValue));
}

// MOVE to Utilities

String formatExposureTime(int index) {
  float time = exposureTimes[index];
  if (time >= 1.0) {
    return String(time, 0) + "\"";
  } else {
    int denominator = round(1.0 / time);
    return "1/" + String(denominator);
  }
}

//Displays microseconds as ms with one digit after the point
String formatCorrectionValue(int value) {
  return String(value / 1000.0, 1) + " ms";
}