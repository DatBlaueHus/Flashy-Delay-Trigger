#include "AppState.hpp"

bool handleInterrupt = false;

void setupXSyncIn() {
  pinMode(XSYNCPORT, INPUT_PULLUP);
  if (digitalPinToInterrupt(XSYNCPORT) == NOT_AN_INTERRUPT) {
#ifdef DEBUG_PRINT
    Serial.println(F("XSYNCPORT does'nt support interrupts!"));
#endif
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
    if (reportXSyncTime) {
      unsigned long elapsed = xOff - xOn;
      updateInfo(("x-sync for " + String(microsAsMillis(elapsed, 3))).c_str());
    }
  }
}
