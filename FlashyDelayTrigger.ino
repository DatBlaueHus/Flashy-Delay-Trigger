#include "AppState.hpp"
#include "Utilities.hpp"

#include "XSyncIn.hpp"
#include "XSyncOut.hpp"

#include "FlashAnalytics.hpp"

#include "XDelay.hpp"

#include "FlashyDisplay.hpp"

// Port configuration

//analog in
const int flashPort = 0; // analog port zero for flash analytics

//digital in
const int xSyncPort = 2; // port for x-Sync switch input from camera, must support interrupt

const int rotaryEncoder1 = 7; // port for direction 1
const int rotaryEncoder2 = 8; // port for direction 2
const int rotaryEncoderSwitch = 9; // the port to set up for the rotary encoder push button

//digital out
const int xOutPort = 4; // port for x-Output

void setup() {

  setupSerialForDebug();

  setupDisplay();
  setupRotaryDelay(rotaryEncoder1, rotaryEncoder2, rotaryEncoderSwitch);
  setupFlashAnalytics(flashPort);
  setupXSyncIn(xSyncPort);
  setupXSyncOut(xOutPort);

}

void loop() {

  if (xSyncTriggered) {
    xSyncTriggered = false;
    fireDelayedTrigger();
  PRINT("Handle x-sync trigger!");
  }
  handleXTriggerState();
  handleFlashAnalyticsState();
  handleRotaryDelay();
}

