

#define FLIGHT_CONTROL_MAX_ACCEPTED_ANGLE 5
#define FLIGHT_CONTROL_MIN_STARTUP_Z_ACCEL 300
#define FLIGHT_CONTROL_MAX_HOVER_Z_ACCEL 5

  
void startQuad() {
  
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
    
    }
    
    if(abs(getRotationAroundY()) > FLIGHT_CONTROL_MAX_ACCEPTED_ANGLE) {
    
    }
    
    // middle value found
    if(abs(getRotationAroundX()) < FLIGHT_CONTROL_MAX_ACCEPTED_ANGLE
       && abs(getRotationAroundY()) > FLIGHT_CONTROL_MAX_ACCEPTED_ANGLE
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
      
      // wait 50ms to see the changes
      //delay(50);
    }
  }
  
  // was unable to startup -> shutdown the motor
  if(!flightControlReady) {
    pwmMiddleFront = 0;
    pwmMiddleBack = 0;
    pwmMiddleLeft = 0;
    pwmMiddleRight = 0;
  }
  
  setMpu6050FastSampleRate();
}


void landQuad() {
  flightControlReady = false; 
}


void stabelizeHeight() {
  
  // read new mpu6050 values
  readMPU6050Values();
    
  while(abs(getZAcceleration()) > FLIGHT_CONTROL_MAX_HOVER_Z_ACCEL) {
    
    // pwm plus or minus
    if(getZAcceleration() > 0) {
      
    } else {
      
    }
    
    // update pwm middle values
  }
}

