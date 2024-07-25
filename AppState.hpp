#ifndef AppState
#define AppState

//Comment out to remove all Serial.print statements
//#define DEBUG_PRINT

#define APPNAME "®" // Flashy Delay Trigger Logo
#define VERSIONTEMPLATE "¯%s   ©" //version + KB Stamp
#define VERSION "1.0.0"

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

unsigned long currentDelayTime;  // The effective X delay in microseconds, based on the user's settings

//sets the encoder to match the current input state
void setEncoderToState();
void (*encoderSetterXallback)(int);

//User input
enum InputUnit : byte {
  EXPOSUREVALUE,
  MILLISECONDS,
};

InputUnit preferredInputUnit;  // the one that the user has set in the prefs
long millisValue = 0;          // Milliseconds value from the user input
long correctionValue = 0;      // Correction value in microseconds
void saveUserPrefs();

// Defines the fixed list of exposure times (in seconds) and their microsecond equivalents
const float exposureTimes[] = { 1.0 / 320, 1.0 / 250, 1.0 / 200, 1.0 / 160, 1.0 / 125, 1.0 / 100, 1.0 / 80, 1.0 / 60, 1.0 / 50, 1.0 / 40, 1.0 / 30, 1.0 / 25, 1.0 / 20, 1.0 / 15, 1.0 / 13, 1.0 / 10, 1.0 / 8, 1.0 / 6, 1.0 / 5, 1.0 / 4, 1.0 / 3, 1.0 / 2.5, 1.0 / 2, 1.0 / 1.6, 1.0 / 1.3, 1.0, 2.0, 4.0, 8.0 };
const byte exposureCount = sizeof(exposureTimes) / sizeof(exposureTimes[0]);
byte exposureIndex = 0;  // index of the currently selected user Index

// State =======================================================

//The enum of the current InputHandlers
enum InputMode : byte {
  EXPOSURE,    // exposure times as exposure or in milliseconds
  CORRECTION,  // additional correction, which is applied on top of
  PREFS,       // preferences
  SPLASH       // splash screen
};

InputMode currentMode;  // The currently set Input handler

//switches to next screen
void switchToNextMode();

//open preference dialog
void openPrefsDialog();

bool closePrefDialog();

//preference state
//The highlighted element of the dialog
#define PREF_RADIOGROUP 0
#define PREF_SETDEFAULTS 1
#define PREF_OK 2

const int nPrefElements = 3;

bool handlePrefSelection();

InputUnit selectedInputUnit;
int currentlyHighlightedPrefElement;
bool includeUserValues = true;

bool displayNeedsUpdate; //set this to true to trigger a redraw

//Info string, do not set directly, use the setter to set it!!
const char* info = NULL;

#define PRIMARY_SPLASH_INFO_ID 1

#define SECONDARY_SPLASH_INFO_ID 2
#define SECONDARY_SPLASH_INFO "¢ or ¡ button to start!"

//Replaces the info text and triggers a redraw
void updateInfo(const char* newInfo, byte identifier = 0, byte showSeconds = 0);

//Handles business logic for automatic updates of the infor screens
void checkForInfoUpdate();

#endif // AppState
