


void initSerialOutput() {
  
  // configure LED for output
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);
  
  // add serial output
  Serial.begin(BAUD_RATE);  
  
}

void mainSetup() {
  
  // initialize gyroscope, accelerator, pwm pins
  initSerialOutput();
  initMPU6050();
  initPWMControl();
  
  // if the gyroscope is ready -> start hovering
  startQuad();
}

void mainLoop() {
  
//  // only do stuff if all controller are ready
//  if(controllerReady) {
//    
//    if(isPowerLevelOk()) { 
//      // update the pwm based on the gyroscope values
//      stabalizeHorizontalPosition();
//    }
//    else {
//      // if we run out of power -> land quad
//      landQuad();
//    }
//  }
}
