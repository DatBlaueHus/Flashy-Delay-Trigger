#ifndef AppState
#define AppState

//controls Serial printing
#define DEBUG_PRINT

#ifdef DEBUG_PRINT
#define PRINT(x) Serial.println(x)
#else
#define PRINT(x)
#endif

// Define the fixed list of exposure times (in seconds) and their microsecond equivalents
const float exposureTimes[] = {1.0/320, 1.0/250, 1.0/200, 1.0/160, 1.0/125, 1.0/100, 1.0/80, 1.0/60, 1.0/50, 1.0/40, 1.0/30, 1.0/25, 1.0/20, 1.0/15, 1.0/13, 1.0/10, 1.0/8, 1.0/6, 1.0/5, 1.0/4, 1.0/3, 1.0/2.5, 1.0/2, 1.0/1.6, 1.0/1.3, 1.0, 2.0, 4.0, 8.0};
const int exposureCount = sizeof(exposureTimes) / sizeof(exposureTimes[0]);
int exposureIndex = 0;  // index of the currently selected user Index

unsigned long currentDelayTime; // The effective delay in microseconds, based on the user's settings

enum InputMode { 
  EXPOSURE, // exposure times
  CORRECTION, // additional correction, which is applied on top of 
  MILLIS // millisenconds
  };

//User settings
InputMode currentMode = EXPOSURE; // The currently preferred Input mode
long millisValue = 0; // Milliseconds value from the user input
long correctionValue = 0; // Correction value in microseconds


#endif