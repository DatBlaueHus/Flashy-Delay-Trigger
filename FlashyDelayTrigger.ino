#include "AppState.hpp"
#include "Utilities.hpp"

#include "XSyncIn.hpp"
#include "XSyncOut.hpp"
#include "FlashAnalytics.hpp"

void setup() {
  setupSerialForDebug();
  setupLoadUserPrefs();
  setupDisplay();
  setupRotaryInput();
  setupFlashAnalytics();
  setupXSyncIn();
  setupXSyncOut();
}

void loop() {
  if (xSyncTriggered) {
    xSyncTriggered = false;
    fireDelayedTrigger();
  }
  do {
      handleXTriggerState();
  } while (blockUI);
  handleFlashAnalyticsState();
  handleRotaryDelay();
  checkForInfoUpdate();
  updateDisplay();
}
