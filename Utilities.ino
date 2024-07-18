#include "AppState.hpp"

// Prints microseconds as ms, with a leading comment and postPoint digits after the decimal. Adds a linebreak
void printMicrosAsMillis(unsigned long micros, String comment, int postPoint) {
  float millis = micros * 0.001;
  PRINT(comment+millis+postPoint+" ms");
}
