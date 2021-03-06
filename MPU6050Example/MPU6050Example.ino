
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"


#define DEBUG_MPU6050 true
#define MPU_SAMPLE_RATE 9 // sample rate = (1kHz / desired freq) - 1 

MPU6050 mpu;

// MPU control/status variables
bool mpu6050Ready = false;
bool dmpReady = false;  // set true if DMP init was successful

uint8_t mpuIntStatus;
uint8_t devStatus;      // return status after each device operation (0 = success, !0 = error)
uint16_t packetSize;    // expected DMP packet size (default is 42 bytes)
uint16_t fifoCount;     // count of all bytes currently in FIFO
uint8_t fifoBuffer[64]; // FIFO storage buffer

// orientation/motion variables
Quaternion q;           // [w, x, y, z]         quaternion container
VectorInt16 aa;         // [x, y, z]            accel sensor measurements
VectorInt16 aaReal;     // [x, y, z]            gravity-free accel sensor measurements
VectorInt16 aaWorld;    // [x, y, z]            world-frame accel sensor measurements
VectorFloat gravity;    // [x, y, z]            gravity vector
float ypr[3];           // [yaw, pitch, roll]   yaw/pitch/roll container and gravity vector


void initMPU6050() {
    Wire.begin();
    //TWBR = 24; // 400kHz I2C clock (200kHz if CPU is 8MHz)

    Serial.begin(38400);
    while (!Serial);

    // initialize device
    #ifdef DEBUG_MPU6050
      Serial.println(F("Initializing I2C devices..."));
    #endif
    
    mpu.initialize();

    // verify connection
    #ifdef DEBUG_MPU6050
      Serial.println(F("Testing device connections..."));
    #endif    
    Serial.println(mpu.testConnection() ? F("MPU6050 connection successful") : F("MPU6050 connection failed"));

    // load and configure the DMP
    #ifdef DEBUG_MPU6050
      Serial.println(F("Initializing DMP..."));
    #endif
    devStatus = mpu.dmpInitialize();

    // supply your own gyro offsets here, scaled for min sensitivity

    // set the sample rate divider
    mpu.setRate(MPU_SAMPLE_RATE);
    //mpu.setDLPFMode(6);

    if (devStatus == 0) {
        // turn on the DMP, now that it's ready
        #ifdef DEBUG_MPU6050
          Serial.println(F("Enabling DMP..."));
        #endif
        mpu.setDMPEnabled(true);

        // set our DMP Ready flag so the main loop() function knows it's okay to use it
        #ifdef DEBUG_MPU6050
          Serial.println(F("DMP ready! Waiting for first packages..."));
        #endif
        dmpReady = true;

        // get expected DMP packet size for later comparison
        packetSize = mpu.dmpGetFIFOPacketSize();
        
    } else {
        Serial.print(F("DMP Initialization failed (code "));
        Serial.print(devStatus);
        Serial.println(F(")"));
    }
}


void readMPU6050Values() {    
    // if programming failed, don't try to do anything
    if (!dmpReady) return;

    // wait for MPU extra packet(s) available
    while (fifoCount < packetSize) {
      fifoCount = mpu.getFIFOCount();
      mpu6050Ready = true;
    }

    // get INT_STATUS byte
    mpuIntStatus = mpu.getIntStatus();

    // get current FIFO count
    fifoCount = mpu.getFIFOCount();

    // check for overflow (this should never happen unless our code is too inefficient)
    if ((mpuIntStatus & 0x10) || fifoCount == 1024) {
        // reset so we can continue cleanly
        mpu.resetFIFO();
        Serial.println(F("Error: FIFO overflow!"));
          
    // otherwise, check for DMP data ready interrupt (this should happen frequently)
    } else if (mpuIntStatus & 0x02) {
        // wait for correct available data length, should be a VERY short wait
        while (fifoCount < packetSize) fifoCount = mpu.getFIFOCount();

        // read a packet from FIFO
        mpu.getFIFOBytes(fifoBuffer, packetSize);
        
        // track FIFO count here in case there is > 1 packet available
        // (this lets us immediately read more without waiting for an interrupt)
        fifoCount -= packetSize;

        // gyroscope - Euler angles in degrees
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetYawPitchRoll(ypr, &q, &gravity);
        
        // accelerator
        mpu.dmpGetQuaternion(&q, fifoBuffer);
        mpu.dmpGetAccel(&aa, fifoBuffer);
        mpu.dmpGetGravity(&gravity, &q);
        mpu.dmpGetLinearAccel(&aaReal, &aa, &gravity);
        mpu.dmpGetLinearAccelInWorld(&aaWorld, &aaReal, &q);

        Serial.print(F("ypr\t"));
        Serial.print(getYawAngle());
        Serial.print(F("\t"));        
        Serial.print(getRotationAroundX());
        Serial.print(F("\t"));
        Serial.println(getRotationAroundY());
    }
    
}

int8_t getYawAngle() {
  return ypr[0] * 180 / M_PI;
}

int8_t getRotationAroundY() {
  return ypr[1] * 180 / M_PI;
}

int8_t getRotationAroundX() {
  return ypr[2] * 180 / M_PI;
}

int16_t getXAcceleration() {
  return aaWorld.x;
}

int16_t getYAcceleration() {
  return aaWorld.y;
}

int16_t getZAcceleration() {
  return aaWorld.z;
}

bool isMPU6050Available() {
  return dmpReady;
}

/********************************************************/

void setup() {
  initMPU6050();
  // read initial values
  readMPU6050Values();
}


void loop() {
  readMPU6050Values();
}

