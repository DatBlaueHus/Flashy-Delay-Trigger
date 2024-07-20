#ifndef XSyncOut
#define XSyncOut

void setupXSyncOut();
void handleXTriggerState();
void fireDelayedTrigger();

unsigned long startedTriggerAt = 0;  // the clock in microseconds when the trigger was started

#define TRIGGERONTIME 2000  // the time the trigger should stay on in microseconds

#endif