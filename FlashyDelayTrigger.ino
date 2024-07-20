#include "AppState.hpp"
#include "Utilities.hpp"

#include "XSyncIn.hpp"
#include "XSyncOut.hpp"
#include "FlashAnalytics.hpp"
#include "FlashyDisplay.hpp"

void setup() {
  setupSerialForDebug();
  setupRotaryInput();
  setupDisplay();
  setupFlashAnalytics();
  setupXSyncIn();
  setupXSyncOut();
  setupLoadUserPrefs();
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
  updateDisplay();
}

