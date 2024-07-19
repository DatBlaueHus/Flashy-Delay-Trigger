#ifndef AppState
#define AppState

//#define DEBUG_PRINT
#include "DebugPrint.hpp"

#define APPNAME "Flashy Delay Trigger"
#define VERSION "Version 0.0.3"

// Port configuration =======================================================

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

// DATA =======================================================

long currentDelayTime; // The effective X delay in microseconds, based on the user's settings

//User input
enum InputUnit:byte {
  EXPOSUREVALUE,
  MILLISECONDS,
};

InputUnit preferredInputUnit;
long millisValue; // Milliseconds value from the user input
long correctionValue; // Correction value in microseconds
void saveUserPrefs();

// Defines the fixed list of exposure times (in seconds) and their microsecond equivalents
const float exposureTimes[] = {1.0/320, 1.0/250, 1.0/200, 1.0/160, 1.0/125, 1.0/100, 1.0/80, 1.0/60, 1.0/50, 1.0/40, 1.0/30, 1.0/25, 1.0/20, 1.0/15, 1.0/13, 1.0/10, 1.0/8, 1.0/6, 1.0/5, 1.0/4, 1.0/3, 1.0/2.5, 1.0/2, 1.0/1.6, 1.0/1.3, 1.0, 2.0, 4.0, 8.0};
const int exposureCount = sizeof(exposureTimes) / sizeof(exposureTimes[0]);
int exposureIndex = 0;  // index of the currently selected user Index

// State =======================================================

//The enum of the current InputHandlers
enum InputMode:byte { 
  EXPOSURE, // exposure times as exposure or in milliseconds
  CORRECTION, // additional correction, which is applied on top of 
  PREFS, // preferences
  SPLASH // splash screen
  };

InputMode currentMode; // The currently set Input handler


#endif