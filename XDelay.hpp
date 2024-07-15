#ifndef XDelay
#define XDelay

enum inputModes { 
  EXPOSURE, // exposure times
  CORRECTION, // additional correction, which is applied on top of 
  MILLIS // millisenconds
  };

unsigned long currentDelayTime; // The effective delay in microseconds, based on the user's settings

unsigned long inverseExposure; // State value of `Exposure` as index exposure, i.e. 15 for 1/15 TODO: Support more than one second
long currentCorrection; // the 

bool valueChanged = false;

//int currentMode = inputModes.MILLIS; // the current input mode

#endif