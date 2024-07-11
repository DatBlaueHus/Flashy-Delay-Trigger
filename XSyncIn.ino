
int xInput = 10; // x-Sync input switch
unsigned long xOn = 0;

volatile byte xInputState = HIGH;
volatile byte xInputPreviousState = HIGH;


void setupXSyncIn() {
  pinMode(xInput, INPUT_PULLUP);
//  attachInterrupt(digitalPinToInterrupt(xInput), handleXLow, FALLING); //we need to attach an interrupt
//  attachInterrupt(digitalPinToInterrupt(xInput), handleXHigh, RISING); //we need to attach an interrupt
}

void reportXSyncState() {
  xInputState = digitalRead(xInput);
  if (xInputState != xInputPreviousState) {
    if (xInputState == LOW) {
      xOn = micros();
    }
    else {
          unsigned long now = micros();         // now: timestamp
          unsigned long elapsed = now - xOn;  // elapsed: duration
          float millis = elapsed * 0.001;
          Serial.print("x-sync-Interval: ");
          Serial.print(millis,2);
          Serial.println(" ms");
    }
    xInputPreviousState = xInputState;
  }
}

// doesn't work

void handleXHigh() {
    //Serial.println("handleXInput");
    xInputState = HIGH;
}

void handleXLow() {
    //Serial.println("handleXInput");
    xInputState = LOW;
 /*   if (xInputState = LOW) {
      xOn = micros();
      Serial.println("x on");
    }
    else {
          unsigned long now = micros();         // now: timestamp
          unsigned long elapsed = now - xOn;  // elapsed: duration
          Serial.print("x-sync-Interval: ");
          Serial.print(elapsed);
          Serial.println(" micros");
    }
    */
}
