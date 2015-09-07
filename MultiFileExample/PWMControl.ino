

#define PWM_FRONT 3  // pwm output pin
#define PWM_RIGHT 5  // pwm output pin
#define PWM_BACK 6   // pwm output pin
#define PWM_LEFT 9   // pwm output pin

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
  
  // don't do anything if MPU6050 is not available
  if(mpu6050Ready) {
    
    Serial.println(F("PWM Control ready"));
  } else {
    pwmRange = 0;
    
    pwmMiddleFront = 0;
    pwmMiddleBack = 0;
    pwmMiddleLeft = 0;
    pwmMiddleRight = 0;
    
    Serial.println(F("Can't initialize PWM Control! MPU6050 is not available"));
  }
  
  // set pwm to 0 -> no power
  analogWrite(PWM_FRONT, 0);
  analogWrite(PWM_RIGHT, 0);
  analogWrite(PWM_BACK, 0);
  analogWrite(PWM_LEFT, 0);  
}


void updatePWMControl() {
  
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
  return MAX_PWM_VALUE - pwmRange;
}

