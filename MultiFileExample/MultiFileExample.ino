



void setup()
{
  initMPU6050();
  initPWMControl();
}


void loop()
{
  readMPU6050Values();
  updatePWMControl();
  
  //printMPU6050Values();
}
