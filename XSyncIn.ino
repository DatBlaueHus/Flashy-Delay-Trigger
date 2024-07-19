#include "AppState.hpp"

bool handleInterrupt = false;

void setupXSyncIn() {
  pinMode(XSYNCPORT, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(XSYNCPORT), handleChange, CHANGE);  //we need to attach an interrupt
  delay(1);
  handleInterrupt = true;
}

void handleChange() {
  if (!handleInterrupt) {
    return;
  }

  xInputState = digitalRead(XSYNCPORT);
  xSyncTriggered = !xInputState;  // trigger goes to LOW
  if (xSyncTriggered) {
    xOn = micros();
  } else {
    xOff = micros();
    if (reportXSyncTime) {
      unsigned long elapsed = xOff - xOn;
      DEBUG_PRINT("x-sync-Interval: " + microsAsMillis(elapsed));
    }
  }
}
