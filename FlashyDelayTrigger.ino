#include "Utilities.hpp"

#include "XSyncIn.hpp"
#include "XSyncOut.hpp"

#include "FlashAnalytics.hpp"

#include "XDelay.hpp"

#include "FlashyDisplay.hpp"

// Port configuration

//analog in
int flashPort = 0; // analog port zero for flash analytics


//digital in
int xSyncPort = 2; // port for x-Sync switch input from camera, must support interrupt

int rotaryEncoder1 = 7; // port for direction 1
int rotaryEncoder2 = 8; // port for direction 2
int rotaryEncoderSwitch = 9; // the port to set up for the rotary encoder push button

//digital out
int xOutPort = 4; // port for x-Output

void setup() {

  if (Serial.availableForWrite()) {
    Serial.begin(115200);
    while (! Serial);
  }

  setupDisplay();

  Serial.println("FlashyDelayTrigger V 0.1");

  setupRotaryDelay(rotaryEncoder1, rotaryEncoder2, rotaryEncoderSwitch);
  setupFlashAnalytics(flashPort);
  setupXSyncIn(xSyncPort);
  setupXSyncOut(xOutPort);

}

void loop() {

  if (xSyncTriggered) {
    xSyncTriggered = false;
    fireDelayedTrigger();
//    Serial.println("Handle x-sync trigger!");
  }
  handleXTriggerState();
  handleFlashAnalyticsState();
  handleRotaryDelay();
}

