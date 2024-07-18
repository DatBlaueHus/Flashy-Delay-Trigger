#include "AppState.hpp"

//This sets up Serial only in Debug mode. It's a noop in builds that haven't define DEBUG_PRINT
void setupSerialForDebug() {
  #ifdef DEBUG_PRINT
    if (Serial.availableForWrite()) {
    Serial.begin(115200);
    while (! Serial);
  }
  #endif
}