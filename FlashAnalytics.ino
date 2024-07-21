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

#ifdef DEBUG_PRINT
  Serial.println("SensRef: " + String(sensRef));
  Serial.println("sensMin: " + String(sensMin));
  Serial.println("sensMax: " + String(sensMax));
#endif
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
#ifdef DEBUG_PRINT
  Serial.println(F("FAST ADC enabled"));
#endif  //DEBUG_PRINT
#endif  //FASTADC

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
      if (xOn == 0) {
          updateInfo("\xFB for " + String(microsAsMillis(now - flashStartedAt, 3)));
      }
      else {
          updateInfo(String(microsAsMillis(flashStartedAt - xOn)) + "\xFB for " + String(microsAsMillis(now - flashStartedAt, 3)));
      }
      xOn = 0;
    }
  }
}
