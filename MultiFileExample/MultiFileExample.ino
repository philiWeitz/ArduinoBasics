

#define LED_PIN 13
#define BAUD_RATE 38400


void initMain() {
  // configure LED for output
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  // add serial output
  Serial.begin(BAUD_RATE);  
}

void setup()
{  
  initMain();
  initMPU6050();
  initPWMControl();
}


void loop()
{
  isPowerLevelOk();
  
  //readMPU6050Values();
  //updatePWMControl();
  
  //printMPU6050Values();
}
