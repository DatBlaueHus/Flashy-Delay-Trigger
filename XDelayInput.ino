#include <RotaryEncoder.h>
#include <EEPROM.h>

#include "XDelay.hpp"
#include "FlashyDisplay.hpp"

RotaryEncoder *encoder;

/*
1″, 1/1.3, 1/1.6, 1/2, 1/2.5, 1/3, 1/4, 1/5, 1/6, 1/8, 1/10, 1/13, 1/15, 1/20, 1/25, 1/30, 1/40, 1/50, 1/60, 1/80, 1/100, 1/125, 1/160, 1/200, 1/250, 1/320, 1/400, 1/500, 1/640, 1/800, 1/1000, 1/1250, 1/1600, 1/2000, 1/2500, 1/3200, 1/4000
*/


// Define the fixed list of exposure times (in seconds) and their microsecond equivalents
const float exposureTimes[] = {1.0/250, 1.0/125, 1.0/60, 1.0/30, 1.0/15, 1.0/8, 1.0/4, 1.0/2, 1.0, 2.0, 4.0};
const unsigned long exposureTimesMicroseconds[] = {4000, 8000, 16667, 33333, 66667, 125000, 250000, 500000, 1000000, 2000000, 4000000};
const int exposureCount = sizeof(exposureTimes) / sizeof(exposureTimes[0]);

int currentIndex = 0;  // To keep track of the current exposure time

//persitsing & debouncing
const int EEPROMAddress = 0; // EEPROM address to store the current exposure time index
unsigned long lastDebounceTime = 0;  // the last time the output was toggled
const unsigned long debounceDelay = 100;    // the debounce time in ms; increase if the output flickers

void setupRotaryDelay(int rotaryPort1, int rotaryPort2, int switchPort) {
  // Pullup the 3 ports
  pinMode(rotaryPort1, INPUT_PULLUP);
  pinMode(rotaryPort2, INPUT_PULLUP);
  pinMode(switchPort, INPUT_PULLUP);

  // Setup a RotaryEncoder with 2 steps per latch for the 2 signal input pins:
  encoder = new RotaryEncoder(rotaryPort1, rotaryPort2, RotaryEncoder::LatchMode::FOUR3);

  // Load the last saved exposure time index from EEPROM
  currentIndex = EEPROM.read(EEPROMAddress);
  if (currentIndex < 0 || currentIndex >= exposureCount) {
    currentIndex = 0;  // Default to the first exposure time if the index is out of range
  }

    // Set the encoder position to the saved position
  encoder->setPosition(currentIndex);
  
  // Print the initial exposure time
  Serial.println("Initial exposure time: 1/" + String(1.0/exposureTimes[currentIndex]));
  Serial.println("Equivalent time in microseconds: " + String(exposureTimesMicroseconds[currentIndex]));
}

// Interrupt service routine to handle encoder updates
void updateEncoder() {
  encoder->tick();  // Update the encoder state

  if (millis() - lastDebounceTime > debounceDelay) {
    int newPos = encoder->getPosition();
    
    // Wrap around if the position goes out of bounds
    if (newPos < 0) {
      newPos = exposureCount - 1;
      encoder->setPosition(newPos);
    } else if (newPos >= exposureCount) {
      newPos = 0;
      encoder->setPosition(newPos);
    }
  
     // Use modulo math to wrap around the position
    newPos = (newPos + exposureCount) % exposureCount;

    if (newPos != currentIndex) {
      currentIndex = newPos;
      EEPROM.write(EEPROMAddress, currentIndex);
      valueChanged = true; // Set the flag to indicate a change
    }
  }  
}

void handleRotaryDelay() {
  updateEncoder();
  if (valueChanged) {
    // Print the new exposure time
    displayText(formatExposureTime(currentIndex));
    Serial.println( "Selected exposure time: " + formatExposureTime(currentIndex) );
    valueChanged = false; // Reset the flag
  }
}

String formatExposureTime(int index) {
  float time = exposureTimes[index];
  if (time >= 1.0) {
    return String(time, 0) + "\"";
  } else {
    int denominator = round(1.0 / time);
    return "1/" + String(denominator);
  }
}