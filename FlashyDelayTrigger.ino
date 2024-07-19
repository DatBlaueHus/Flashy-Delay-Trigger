#include "AppState.hpp"
#include "Utilities.hpp"

#include "XSyncIn.hpp"
#include "XSyncOut.hpp"
#include "XDelay.hpp"
#include "FlashAnalytics.hpp"
#include "FlashyDisplay.hpp"

void setup() {

  setupSerialForDebug();
  setupLoadUserPrefs();
  setupDisplay();
  setupRotaryDelay();
  setupFlashAnalytics();
  setupXSyncIn();
  setupXSyncOut();
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

