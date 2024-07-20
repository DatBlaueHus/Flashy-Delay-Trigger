#include "AppState.hpp"
#include "FlashAnalytics.hpp"
#include "XSyncOut.hpp"
#include "XSyncOut.hpp"

bool flashIsOn = false;  // state var to store the flash value

//Fast settings for analog read!
#define FASTADC 1

// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif

//measured base level
void setReferenceLevel() {
  int sens = analogRead(FLASHPORT);
  int sensMax = sens;
  int sensMin = sens;

  for (int x = 0; x < 500; x++) {
    sens = analogRead(FLASHPORT);
    if (sens > sensMax) {
      sensMax = sens;
    }
    if (sens < sensMin) {
      sensMin = sens;
    }
  }
  sensRef = sensMax + 10;  //TODO: Rather go for an activation on the raise versus the previous level

  PRINT("SensRef: " + String(sensRef));
  PRINT("sensMin: " + String(sensMin));
  PRINT("sensMax: " + String(sensMax));
}

void setupFlashAnalytics() {
  int x = 0;
  int sens = 0;
  int sensMax = 0;
  int sensMin = 0;

#if FASTADC
  // set prescale to 16 t allow faster sampling
  sbi(ADCSRA, ADPS2);
  cbi(ADCSRA, ADPS1);
  cbi(ADCSRA, ADPS0);
  PRINT("FAST ADC enabled");
#endif

setReferenceLevel();
}

//handle the flash state, returns true if the flash was started
void handleFlashAnalyticsState() {

  if (analogRead(FLASHPORT) > sensRef) {
    if (!flashIsOn) {
      flashStartedAt = micros();  // save timestamp for beginning of flash
      flashIsOn = true;
    }
    //TODO: If the sensor get's activated for more than 5 seconds, recalibrate!
  } else {
    if (flashIsOn) {
      unsigned long now = micros();
      flashIsOn = false;
      PRINT("Actual Flash delay: " + microsAsMillis(flashStartedAt - xOn));
      PRINT("duration: " + microsAsMillis(now - flashStartedAt, 3));
    }
  }
}
