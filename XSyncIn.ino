int xInput = 2; // Port for x-Sync switch, check if port actually supports Interrupts, i.e. Arduino nano only supports port 2 & 3!
bool handleInterrupt = false;

void setupXSyncIn(int inputPort) {
  xInput = inputPort;
  pinMode(xInput, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(xInput), handleChange, CHANGE); //we need to attach an interrupt
  delay(1);
  handleInterrupt = true;
}

void handleChange() {
  if (!handleInterrupt) {
    return;
  }

  xInputState = digitalRead(xInput);
  xSyncTriggered = !xInputState; // trigger goes to LOW
  if (xSyncTriggered) {
     xOn = micros();
  }
  else {
    xOff = micros();
    if (reportXSyncTime) {
      unsigned long elapsed = xOff - xOn;
      printMicrosAsMillis(elapsed, "x-sync-Interval: ");
    }
  }
}
