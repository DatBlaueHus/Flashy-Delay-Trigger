#include "AppState.hpp"
#include "XSyncOut.hpp"

bool triggerIsOn = false;          // State of the trigger
bool delayIsOn = false;            // State of the delay
unsigned long delayStartTime = 0;  // the timestamp when the delay was started
unsigned long triggerTime = 2000;  // the time the trigger should stay on in microseconds

void setupXSyncOut() {
  pinMode(XOUTPORT, OUTPUT);
}

// Fires the trigger with the delay given in `triggerDelay` in ms. Note that yo must call `handleXTriggerState()`in your loop to actually fire the trigger!
void fireDelayedTrigger() {
  PRINT("Start Trigger Delay!");
  delayStartTime = micros();
  delayIsOn = true;
}

// Fires the trigger immediately
void fireXTrigger() {
  PRINT("Fire Trigger!");
  digitalWrite(XOUTPORT, HIGH);
  startedTriggerAt = micros();
  triggerIsOn = true;
}

// handles Trigger deactivation
void handleXTriggerState() {
  if (delayIsOn) {
    unsigned long now = micros();
    unsigned long interval = now - delayStartTime;
    if (interval > currentDelayTime) {
      digitalWrite(XOUTPORT, HIGH);
      startedTriggerAt = micros();
      delayIsOn = false;
      triggerIsOn = true;
      PRINT("Fire delayed trigger ***!");
    }
  } else if (triggerIsOn) {
    unsigned long now = micros();
    unsigned long interval = now - startedTriggerAt;
    if (interval > triggerTime) {
      digitalWrite(XOUTPORT, LOW);
      triggerIsOn = false;
      PRINT("Trigger is off again!");
    }
  }
}