#include "FlashAnalytics.hpp"
#include "XSyncOut.hpp"

int inputPort;

bool flashIsOn = false; // state var to store the flash value

//Fast settings for analog read!
#define FASTADC 1

// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif


void setupFlashAnalytics(int analogPort) {
  int x = 0;
  int sens = 0;
  int sensMax = 0;
  
  inputPort = analogPort;
  
  #if FASTADC
    // set prescale to 16
    sbi(ADCSRA,ADPS2);
    cbi(ADCSRA,ADPS1);
    cbi(ADCSRA,ADPS0);
  #endif

    for(x =0; x < 500; x++) {
      sens = analogRead(inputPort);
      if (sens > sensMax) {
          sensMax = sens;
      }
    }
  
  sensRef = sensMax + 10;

  #if FASTADC
    PRINT("FAST ADC");
  #endif
  PRINT("SensRef: "+sensRef);
}

//handle the flash state, returns true if the flash was started
void handleFlashAnalyticsState() {
  
  if (analogRead(0) > sensRef) {
    if (!flashIsOn) {
      flashStartedAt = micros();         // save timestamp for beginning of flash
      flashIsOn = true;
    }
  }
  else {
    if (flashIsOn) {
        unsigned long now = micros();
        flashIsOn = false;
        printMicrosAsMillis(flashStartedAt - xOn, "Actual Flash delay: ");
        printMicrosAsMillis(now - flashStartedAt, "duration: ",3);
    }
  }
  
}
