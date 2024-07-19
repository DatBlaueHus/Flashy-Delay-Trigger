#include "AppState.hpp"

// Prints microseconds as ms, with a leading comment and postPoint digits after the decimal. Adds a linebreak
void printMicrosAsMillis(unsigned long micros, String comment, int postPoint) {
  float millis = micros * 0.001;
  PRINT(comment + millis + postPoint + " ms");
}

// Formatters ==================================================================================================

String formatExposureTime(int index) {
  float time = exposureTimes[index];
  if (time >= 1.0) {
    return String(time, 0) + "\"";
  } else {
    float denominator = 1.0 / time;
    int roundedDenominator = (int)(denominator + 0.5);  // Properly round the denominator

    // Check if the rounded denominator is close enough to the actual denominator
    if (abs(denominator - roundedDenominator) > 0.01) {
      return "1/" + String(denominator, 1);
    } else {
      return "1/" + String(roundedDenominator);
    }
  }
}

//Displays microseconds as ms with one digit after the point
String formatCorrectionValue(int value) {
  return String(value / 1000.0, 1) + "ms";
}

//Converters ==================================================================================================

long calculateExposureMicroseconds(int index) {
  return static_cast<long>(exposureTimes[index] * 1000000);
}

//calculates the milliseconds from the current exposure value
long millisFromExposure() {
  return calculateExposureMicroseconds(exposureIndex) / 1000;
}

int exposureFromMillis() {
  return findNearestExposureIndex(millisValue);
}

int findNearestExposureIndex(long milliseconds) {
  long microseconds = milliseconds * 1000;
  int nearestIndex = 0;
  long smallestDifference = abs(calculateExposureMicroseconds(0) - microseconds);
  for (int i = 1; i < exposureCount; i++) {
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
