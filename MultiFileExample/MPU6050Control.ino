

#include "Wire.h"

#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"

#define BAUD_RATE 38400
#define LED_PIN 13


MPU6050 mpu;


// MPU control/status vars
bool dmpReady = false;  // set true if DMP init was successful
uint8_t mpuIntStatus;   // holds actual interrupt status byte from MPU
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion vars
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector


void initMPU6050() {
  
  // configure LED for output
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  
  Wire.begin();
  Serial.begin(BAUD_RATE);

  // initialize device
  Serial.println(F("Initializing MPU-6050..."));
  mpu.initialize();

  // verify connection
  Serial.println(F("Testing device connections..."));
  devStatus = mpu.testConnection();
  
  if(0 == devStatus) {
    Serial.println(F("MPU-6050 connection successful"));
  } else {
    Serial.print(F("MPU-6050 connection failed! Error code: "));
    Serial.println(devStatus);
  }

  // load and configure the DMP
  Serial.println(F("Initializing DMP..."));
  devStatus = mpu.dmpInitialize();

  if (devStatus == 0) {
    // turn on the DMP, now that it's ready
    Serial.println(F("Enabling DMP..."));
    mpu.setDMPEnabled(true);
    mpuIntStatus = mpu.getIntStatus();

    // set our DMP Ready flag so the main loop() function knows it's okay to use it
    Serial.println(F("DMP ready! Waiting for first interrupt..."));
    dmpReady = true;
    digitalWrite(LED_PIN, HIGH);
    
    // get expected DMP packet size for later comparison
    packetSize = mpu.dmpGetFIFOPacketSize();
    
  } else {
    Serial.print(F("DMP Initialization failed! Error code: "));
    Serial.println(devStatus);
  }
}


void readMPU6050Values() {
  
  // if programming failed, don't try to do anything
  if (!dmpReady) return;

  mpuIntStatus = mpu.getIntStatus();
  // get current FIFO count
  fifoCount = mpu.getFIFOCount();

  if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
    Serial.println(F("FIFO Overflow"));
    mpu.resetFIFO();
  
} else if (mpuIntStatus & 0x02) {    
    // wait for correct available data length, should be a VERY short wait
    while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

    // read a packet from FIFO
    mpu.getFIFOBytes(fifoBuffer, packetSize);

    // track FIFO count here in case there is > 1 packet available
    // (this lets us immediately read more without waiting for an interrupt)
    fifoCount -= packetSize;

    // gyroscope
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
    
    // accelerator
    mpu.dmpGetQuaternion(&q, fifoBuffer);
    mpu.dmpGetAccel(&aa, fifoBuffer);
    mpu.dmpGetGravity(&gravity, &q);
    mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
    mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);
  }
}


float getYawAngle() {
  return ypr[0] * 180 / M_PI;
}

float getPitchAngle() {
  return ypr[1] * 180 / M_PI;
}

float getRollAngle() {
  return ypr[2] * 180 / M_PI;
}


uint16_t getXAcceleration() {
  return aaWorld.x;
}

uint16_t getYAcceleration() {
  return aaWorld.y;
}

uint16_t getZAcceleration() {
  return aaWorld.z;
}

bool isMPU6050Available() {
  return dmpReady;
}

void printMPU6050Values() {
  
  // gyroscope
  Serial.print(F("Gyroscope \t\t yaw: "));
  Serial.print(getYawAngle());
  Serial.print(F(" - pitch: "));
  Serial.print(getPitchAngle());
  Serial.print(F(" - roll: "));
  Serial.println(getRollAngle());

  // accelerator  
  /*Serial.print("Accelerator \t\t x: ");
  Serial.print(getXAcceleration());
  Serial.print(" y: ");
  Serial.print(getYAcceleration());
  Serial.print(" z: ");
  Serial.println(getZAcceleration());*/
}



