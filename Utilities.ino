#include "AppState.hpp"

// Prints microseconds as ms, with a leading comment and postPoint digits after the decimal. Adds a linebreak
String microsAsMillis(long microseconds, int postPoint) {
  String res = String(microseconds / 1000.0, postPoint) + "¿"; //¿ = ms in my custom font
  return res;
}

// Formatters ==================================================================================================

//transforms the string by 130 to get to special our small numbers
void transformString(char *str) {
  while (*str) {
    *str = *str + 130;  // Transform '.' to '°' assuming Latin-1 Supplement
    str++;
  }
}

String formatMilliseconds(long millis) {
  return String(millis) + "¿"; // ¿ = ms in my custom font
}

//Create the small version of the exposure time for the preset screen
String formatSmallExposureTime(byte index) {
  float time = exposureTimes[index];
  if (time >= 1.0) {
    return String(time, 0) + "\"";
  } else {
    float denominator = 1.0 / time;
    int roundedDenominator = (int)(denominator + 0.5);  // Properly round the denominator
    // Check if the rounded denominator is close enough to the actual denominator
    String rawString;
    if (abs(denominator - roundedDenominator) > 0.01) {
      rawString = String(denominator, 1);
    } else {
      rawString = String(roundedDenominator);
    }
    char cStr[rawString.length() + 1];          // Create a buffer for the C-string
    rawString.toCharArray(cStr, sizeof(cStr));  // Convert String to C-string
    transformString(cStr);
    rawString = String(cStr);
    return "±" + rawString;
  }
}

String formatExposureTime(byte index) {
  String changed = millisValue != calculateExposureMicroseconds(exposureIndex) / 1000 ? "~" : "";
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

//Converters ==================================================================================================

long calculateExposureMicroseconds(byte index) {
  return static_cast<long>(exposureTimes[index] * 1000000);
}

//calculates the milliseconds from the current exposure value
long millisFromExposure() {
  return calculateExposureMicroseconds(exposureIndex) / 1000;
}

int exposureFromMillis() {
  return findNearestExposureIndex(millisValue);
}

byte findNearestExposureIndex(long milliseconds) {
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
