

#define MAX_PWM_VALUE 50
#define E 2.718281828459045235360287471352

#define ROLL_X abs(getRotationAroundY()/90)
#define PITCH_X abs(getRotationAroundX()/90)

// f(x) = e^((x/0.2)-5)
// f(x)' = (1/0.2) * e^((x/0.2)-5)
#define GET_PWM_VALUE(x) min(1,(1/0.2) * pow(E,((x/0.2)-5)));

float pwmValue = 0;


void initPWMControl() {
  Serial.println(F("Initializing PWM Control..."));
  
  if(mpu6050Ready) {
    // if PMU ready -> set initial values to PWM
    Serial.println(F("PWM Control ready"));
  } else {
    // don't do anything if MPU6050 is not available
    pwmRange = 0;
    Serial.println(F("Can't initialize PWM Control! MPU6050 is not available"));
  }
  
  // initially all values are set to 0
  setPwmMiddleValues();
}


void setPwmMiddleValues() {
  analogWrite(PWM_FRONT, pwmMiddleFront);
  analogWrite(PWM_RIGHT, pwmMiddleRight);
  analogWrite(PWM_BACK, pwmMiddleBack);
  analogWrite(PWM_LEFT, pwmMiddleLeft);  
}


void stabalizeHorizontalPosition() {
  
  // read new mpu6050 values
  readMPU6050Values();
    
  // roll angle     
  pwmValue = GET_PWM_VALUE(ROLL_X);
  
  if(getRotationAroundY() > 0) {
    // left is down
    analogWrite(PWM_LEFT, pwmMiddleLeft + (pwmValue * pwmRange));
    analogWrite(PWM_RIGHT, pwmMiddleRight - (pwmValue * pwmRange));
  } else  {
    // right is down
    analogWrite(PWM_RIGHT, pwmMiddleRight + (pwmValue * pwmRange));
    analogWrite(PWM_LEFT, pwmMiddleLeft - (pwmValue * pwmRange));
  }

  // pitch angle 
  pwmValue = GET_PWM_VALUE(PITCH_X);
  
  if(getRotationAroundX() > 0) {
    // nose is down
    analogWrite(PWM_FRONT, pwmMiddleFront + (pwmValue * pwmRange));
    analogWrite(PWM_BACK, pwmMiddleBack - (pwmValue * pwmRange));
  } else  {
    // back is down
    analogWrite(PWM_BACK, pwmMiddleBack + (pwmValue * pwmRange));
    analogWrite(PWM_FRONT, pwmMiddleFront - (pwmValue * pwmRange));
  }
}

uint8_t getMaxPwmValue() {
  return MAX_PWM_VALUE;
}

