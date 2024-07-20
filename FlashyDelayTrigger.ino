#include "AppState.hpp"
#include "Utilities.hpp"

#include "XSyncIn.hpp"
#include "XSyncOut.hpp"
#include "FlashAnalytics.hpp"
#include "FlashyDisplay.hpp"

void setup() {
  setupSerialForDebug();
  setupLoadUserPrefs();
  setupRotaryInput();
  setupDisplay();
  setupFlashAnalytics();
  setupXSyncIn();
  setupXSyncOut();
}

void loop() {
  if (xSyncTriggered) {
    xSyncTriggered = false;
    fireDelayedTrigger();
  }
  handleXTriggerState();
  handleFlashAnalyticsState();
  handleRotaryDelay();
  updateDisplay();
}
