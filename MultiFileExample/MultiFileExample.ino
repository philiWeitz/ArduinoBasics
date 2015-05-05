



void setup()
{
  initMPU6050();
}


void loop()
{
  readMPU6050Values();
  printMPU6050Values();
}
