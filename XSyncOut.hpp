#ifndef XSyncOut
#define XSyncOut

void setupXSyncOut();
void handleXTriggerState();
void fireDelayedTrigger();

unsigned long startedTriggerAt = 0;  // the clock in microseconds when the trigger was started
unsigned long triggerDelay = 60000;  // the delay to add to the trigger signal

#endif