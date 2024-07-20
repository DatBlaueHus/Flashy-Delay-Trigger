#ifndef FlashAnaLyticsHeader
#define FlashAnaLyticsHeader

unsigned long flashStartedAt = 0;  // the clock in microseconds when the beginning of the flash was measured
unsigned long flashDownT5 = 0;  // the clock in microseconds when the flash level is at 50 (linearity unchecked!!)
unsigned long flashDownT1 = 0;  // the clock in microseconds when the flash level is at 10 (linearity unchecked!!)

int sensRef = 0;  // The reference reading level

#endif