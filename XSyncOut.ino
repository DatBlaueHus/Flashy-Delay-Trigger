#include "AppState.hpp"
#include "XSyncOut.hpp"

bool triggerIsOn = false;          // State of the trigger
bool delayIsOn = false;            // State of the delay
unsigned long delayStartTime = 0;  // the timestamp when the delay was started

void setupXSyncOut() {
  pinMode(XOUTPORT, OUTPUT);
  digitalWrite(XOUTPORT, LOW);
}

// Fires the trigger with the delay given in `triggerDelay` in ms. Note that yo must call `handleXTriggerState()`in your loop to actually fire the trigger!
void fireDelayedTrigger() {
  delayStartTime = micros();
  delayIsOn = true;
  blockUI = true;
}

// Fires the trigger immediately
void fireXTrigger() {
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
      fireXTrigger();
#ifdef DEBUG_PRINT
      Serial.println(F("Delayed trigger fired!! **"));
#endif
      setEncoderToState();  //kill any intermediate user input
      blockUI = false;
      delayIsOn = false;
      triggerIsOn = true;
    }
  } else if (triggerIsOn) {
    unsigned long now = micros();
    unsigned long interval = now - startedTriggerAt;
    if (interval > TRIGGERONTIME) {
      digitalWrite(XOUTPORT, LOW);
#ifdef DEBUG_PRINT
      Serial.println(F("Trigger switched off!"));
#endif
      triggerIsOn = false;
    }
  }
}
