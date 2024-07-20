#ifndef XSyncOut
#define XSyncOut

void setupXSyncOut();
void handleXTriggerState();
void fireDelayedTrigger();

unsigned long startedTriggerAt = 0;  // the clock in microseconds when the trigger was started

#endif