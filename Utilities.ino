#include "AppState.hpp"

// Prints microseconds as ms, with a leading comment and postPoint digits after the decimal. Adds a linebreak
String microsAsMillis(long microseconds, int postPoint) {
  String res = String(microseconds / 1000.0, postPoint) + " ms";
  #ifdef DEBUG_PRINT
  Serial.println(res);
  #endif
  return res;
}

// Formatters ==================================================================================================

String formatExposureTime(byte index) {
  String changed = millisValue != calculateExposureMicroseconds(exposureIndex) / 1000 ? "\xFA" : "";
  float time = exposureTimes[index];
  if (time >= 1.0) {
    return changed + String(time, 0) + "\"";
  } else {
    float denominator = 1.0 / time;
    int roundedDenominator = (int)(denominator + 0.5);  // Properly round the denominator

    // Check if the rounded denominator is close enough to the actual denominator
    if (abs(denominator - roundedDenominator) > 0.01) {
      return changed + "1/" + String(denominator, 1);
    } else {
      return changed + "1/" + String(roundedDenominator);
    }
  }
}

//Displays microseconds as ms with one digit after the point
String formatCorrectionValue(int value) {
  return String(value / 1000.0, 1) + "ms";
}

//Converters ==================================================================================================

unsigned long calculateExposureMicroseconds(byte index) {
  return static_cast<unsigned long>(exposureTimes[index] * 1000000);
}

//calculates the milliseconds from the current exposure value
unsigned long millisFromExposure() {
  return calculateExposureMicroseconds(exposureIndex) / 1000;
}

int exposureFromMillis() {
  return findNearestExposureIndex(millisValue);
}

byte findNearestExposureIndex(unsigned long milliseconds) {
  long microseconds = milliseconds * 1000;
  byte nearestIndex = 0;
  long smallestDifference = abs(calculateExposureMicroseconds(0) - microseconds);
  for (byte i = 1; i < exposureCount; i++) {
    long difference = abs(calculateExposureMicroseconds(i) - microseconds);
    if (difference < smallestDifference) {
      smallestDifference = difference;
      nearestIndex = i;
    } else {
      return nearestIndex;  // as soon as the difference is bigger, we can bail out!
    }
  }
  return nearestIndex;
}
