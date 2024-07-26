#include "AppState.hpp"

bool handleInterrupt = false;

void setupXSyncIn() {
  pinMode(XSYNCPORT, INPUT_PULLUP);
  if (digitalPinToInterrupt(XSYNCPORT) == NOT_AN_INTERRUPT) {
      updateInfo("ÂNo interrupts on X-IN", ERROR_INFO_ID, 10);
  } else {
    attachInterrupt(digitalPinToInterrupt(XSYNCPORT), handleChange, CHANGE);  //we need to attach an interrupt
    delay(1);
    handleInterrupt = true;
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
    unsigned long elapsed = xOff - xOn;
    updateInfo(("¾ x-sync " + String(microsAsMillis(elapsed, 3))).c_str());
  }
}
