#include "Utilities.hpp"

#include "XSyncIn.hpp"
#include "XSyncOut.hpp"

#include "FlashAnalytics.hpp"


int ledPort = 13; // LED output
int flashPort = 0; // analog port zero for flash analytics

int xSyncPort = 2; // port for x-Sync switch inputr from camera
int xOutPort = 4; // port for x-Output

bool flashStarted = false;

void setup() {

  setupFlashAnalytics(flashPort);
  setupXSyncIn(xSyncPort);
  setupXSyncOut(xOutPort);

  if (Serial.availableForWrite()) {
    Serial.begin(9600);
    Serial.println("Configuration");
 #if FASTADC
   Serial.println("FAST ADC");
  #endif
    Serial.print("SensRef: ");
    Serial.println(sensRef);
  }
}

void loop() {

  if (xSyncTriggered) {
    xSyncTriggered = false;
    fireDelayedTrigger();
//    Serial.println("Handle x-sync trigger!");
  }
  handleXTriggerState();
  handleFlashAnalyticsState();
}

