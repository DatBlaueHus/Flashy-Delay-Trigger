#include "AppState.hpp"
#include "FlashAnalytics.hpp"
#include "XSyncOut.hpp"
#include "XSyncOut.hpp"

bool flashIsOn = false;  // state var to store the flash value

//Fast settings for analog read!
#define FASTADC 1

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
  Serial.println("SRef: " + String(sensRef));
  Serial.println("sMin: " + String(sensMin));
  Serial.println("sMax: " + String(sensMax));
#endif
}

void setupFlashAnalytics() {
#if FASTADC
  // set prescale to 16 t allow faster sampling
  // defines for setting and clearing register bits
  #ifndef cbi
  #define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
  #endif
  #ifndef sbi
  #define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
  #endif

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
      if (xOn == 0 || (flashStartedAt - xOn) > currentDelayTime + 5000) {  //very high delays are usually not triggered
          updateInfo(("\xFB for " + String(microsAsMillis(now - flashStartedAt, 3))).c_str());
      }
      else {
          updateInfo((String(microsAsMillis(flashStartedAt - xOn)) + "\xFB for " + String(microsAsMillis(now - flashStartedAt, 3))).c_str());
      }
      xOn = 0;
    }
  }
}
