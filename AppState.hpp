#ifndef AppState
#define AppState

int exposureIndex = 0;  // To keep track of the current exposure time
long millisValue = 0; // Milliseconds value from the user input
long correctionValue = 0; // Correction value in microseconds
unsigned long currentDelayTime; // The effective delay in microseconds, based on the user's settings

#endif