//Fast settings for analog read!
#define FASTADC 1

// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif


int x =0;
int led = 13;
int sens = 0;
int sensMax = 0;
int sensRef = 0;
char charBuf[50];

void setup() {

#if FASTADC
  // set prescale to 16
  sbi(ADCSRA,ADPS2) ;
  cbi(ADCSRA,ADPS1) ;
  cbi(ADCSRA,ADPS0) ;
#endif

  pinMode(led, OUTPUT);

  if (Serial.availableForWrite()) {
    Serial.begin(9600);
    sens = analogRead(0);
//    Serial.print("sensBase: ");
//    Serial.println(sens);
    for(x =0; x < 500; x++) {
      sens = analogRead(0);
      if (sens > sensMax) {
          sensMax = sens;
      }
   }
   sensRef = sensMax + 10;
   sensMax = sensRef;

    Serial.println("Configuration");
 #if FASTADC
   Serial.println("FAST ADC");
  #endif
    Serial.print("SensRef: ");
    Serial.println(sensRef);
    digitalWrite(led, HIGH);
    delay(100);
    digitalWrite(led, LOW);
  }
  else {
    Serial.println("Error: Serial not available!");
  }
}

void loop() {
  sens = analogRead(0);
  if (sens > sensRef) {
    
    //erste Messung speichern
    unsigned long start = micros();
    
    //Abwarten, bis wir wieder drunter kommen
    while (analogRead(0) > sensRef) {};

    unsigned long now = micros();         // now: timestamp
    unsigned long elapsed = now - start;  // elapsed: duration
    Serial.print("Interval: ");
    Serial.print(elapsed);
    Serial.println(" micros");
    delay(300);
  }
}