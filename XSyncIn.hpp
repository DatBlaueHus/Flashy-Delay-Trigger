volatile byte xInputState = HIGH;

bool xSyncTriggered = false; //True if triggered. After handling the trigger, it's safe to set this back to false

unsigned long xOn = 0; // start time for the trigger to LOW
unsigned long xOff = 0; // stop time for the rise back to HIGH

bool reportXSyncTime = true;