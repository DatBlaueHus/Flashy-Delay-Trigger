#include "AppState.hpp"

bool handleInterrupt = false;

void setupXSyncIn() {
  pinMode(XSYNCPORT, INPUT_PULLUP);
  if (digitalPinToInterrupt(XSYNCPORT) == NOT_AN_INTERRUPT) {
    PRINT("XSYNCPORT does not support interrupts! Please adjust the port in ");
  } else {
    attachInterrupt(digitalPinToInterrupt(XSYNCPORT), handleChange, CHANGE);  //we need to attach an interrupt
    delay(1);
    handleInterrupt = true;
    PRINT("XSYNCPORT set up");
  }
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
      PRINT("x-sync-Interval: " + microsAsMillis(elapsed));
    }
  }
}
