#ifndef KBUtilities
#define KBUtilities

//Formats microseconds as ms with one digit after the point
String microsAsMillis(long micros, int postPoint = 2);

//formats the exposure time of the given index for display purposes
String formatExposureTime(int index);

//Create the small version of the exposure time for the preset screen
String formatSmallExposureTime(byte index);

//transforms the string by 130 to get to special our small numbers
void transformString(char *str);

long calculateExposureMicroseconds(byte index);

//calculates the milliseconds from the current exposure value
long millisFromExposure();

//calculates nearest exposure for the current value of millis
int exposureFromMillis();

//calculates nearest exposure for the provided value of millis
byte findNearestExposureIndex(long milliseconds);

#endif
