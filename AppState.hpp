#ifndef AppState
#define AppState

// Port configuration

// analog port zero for flash analytics
#define FLASHPORT 0 

//digital in
// PORT FOR X-SYNC SWITCH INPUT FROM CAMERA, MUST SUPPORT INTERRUPT
#define XSYNCPORT 2
// PORT FOR X-OUTPUT
#define XOUTPORT 4
// PORT FOR DIRECTION 1
#define ROTARYENCODER1 7 
// PORT FOR DIRECTION 2
#define ROTARYENCODER2 8 
// THE PORT TO SET UP FOR THE ROTARY ENCODER PUSH BUTTON
#define ROTARYENCODERSWITCH 9 


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

//The enum of the current InputHandlers
enum InputMode { 
  EXPOSURE, // exposure times as exposure or in milliseconds
  CORRECTION, // additional correction, which is applied on top of 
  PREFS // preferences
  };

InputMode currentMode; // The currently preferred Input mode

enum InputUnit {
  EXPOSUREVALUE,
  MILLISECONDS,
};

//User settings
InputUnit preferredInputUnit;

long millisValue = 0; // Milliseconds value from the user input
long correctionValue = 0; // Correction value in microseconds

void saveUserPrefs();

#endif