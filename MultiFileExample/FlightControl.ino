

#define FLIGHT_CONTROL_MAX_ACCEPTED_ANGLE 5
#define FLIGHT_CONTROL_MIN_STARTUP_Z_ACCEL 300
#define FLIGHT_CONTROL_MAX_HOVER_Z_ACCEL 5

  
void startQuad() {
  
  if(!mpu6050Ready) {
    Serial.println(F("Can't start quad! MPU6050 is not available"));
    return;
  }
  
  Serial.println(F("Staring quad..."));
  setMpu6050SlowSampleRate();
  
  // contains the values which should be add to the pwm 
  uint8_t tmpPwmFront;
  uint8_t tmpPwmBack;
  uint8_t tmpPwmLeft;
  uint8_t tmpPwmRight;  
  
  // if none of the values has reached the maximum values -> continue startup
  while(pwmMiddleFront < getMaxPwmValue() && pwmMiddleBack < getMaxPwmValue()
        && pwmMiddleLeft < getMaxPwmValue() && pwmMiddleRight < getMaxPwmValue()) {
    
    tmpPwmFront = 1;
    tmpPwmBack = 1;
    tmpPwmLeft = 1;
    tmpPwmRight = 1;
    
    // read new mpu6050 values
    readMPU6050Values();
    
    if(abs(getRotationAroundX()) > FLIGHT_CONTROL_MAX_ACCEPTED_ANGLE) {
      if(getRotationAroundX() < 0) {
        
      } else {
        
      }
    }
    
    if(abs(getRotationAroundY()) > FLIGHT_CONTROL_MAX_ACCEPTED_ANGLE) {
      if(getRotationAroundY() < 0) {
        
      } else {
        
      }    
    }
    
    // middle value found
    if(abs(getRotationAroundX()) < FLIGHT_CONTROL_MAX_ACCEPTED_ANGLE
       && abs(getRotationAroundY()) < FLIGHT_CONTROL_MAX_ACCEPTED_ANGLE
       && getZAcceleration() > FLIGHT_CONTROL_MIN_STARTUP_Z_ACCEL) {
        
      stabelizeHeight();
      flightControlReady = true;
      break;
    
    } else {
      // add new pwm value
      pwmMiddleFront += tmpPwmFront;
      pwmMiddleBack += tmpPwmBack;
      pwmMiddleLeft += tmpPwmLeft;
      pwmMiddleRight += tmpPwmRight;
      
      setPwmMiddleValues();
      // wait 1ms to see the changes
      //delay(1);
    }
  }
  
  // was unable to startup -> shutdown the motor
  if(!flightControlReady) {
    pwmMiddleFront = 0;
    pwmMiddleBack = 0;
    pwmMiddleLeft = 0;
    pwmMiddleRight = 0;
    
    setPwmMiddleValues();
    Serial.println(F("Maximum PWM values reached - motor is shutting down"));
  }
  
  setMpu6050FastSampleRate();
}


void landQuad() {
  flightControlReady = false; 
}


void stabelizeHeight() {
  
  Serial.print(F("Stabalizing height..."));
  int8_t val = 0;
      
  while(abs(getZAcceleration()) > FLIGHT_CONTROL_MAX_HOVER_Z_ACCEL) {  
    // read new mpu6050 values
    readMPU6050Values();
  
    // pwm plus or minus
    if(getZAcceleration() > 0) {
      val = -1;
    } else {
      val = 1;   
    }
    
    pwmMiddleFront += val;
    pwmMiddleBack += val;
    pwmMiddleLeft += val;
    pwmMiddleRight += val;   
 
    setPwmMiddleValues();
  }
  
  Serial.println(F("done"));
}

