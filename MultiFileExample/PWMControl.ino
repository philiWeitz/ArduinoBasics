

#define PWM_FRONT 3
#define PWM_RIGHT 5
#define PWM_BACK 6
#define PWM_LEFT 9

#define E 2.718281828459045235360287471352


unsigned int range = 0;
unsigned int middle = 0;

void initPWMControl() {
  Serial.println(F("Initializing PWM Control..."));
  
  // don't do anything if MPU6050 is not available
  if(isMPU6050Available()) {
    
    // TODO: values are only dummy values
    range = 16;
    middle = 16;
    
    Serial.println(F("PWM Control ready"));
  } else {
    range = 0;
    middle = 0;  
    
    Serial.println(F("Can't initialize PWM Control! MPU6050 is not available"));
  }
  
  analogWrite(PWM_FRONT, middle);
  analogWrite(PWM_RIGHT, middle);
  analogWrite(PWM_BACK, middle);
  analogWrite(PWM_LEFT, middle);  
}


float pwmValue = 0;

#define ROLL_X abs(getRollAngle()/90)
#define PITCH_X abs(getPitchAngle()/90)

// f(x) = e^((x/0.2)-5)
// f(x)' = (1/0.2) * e^((x/0.2)-5)
#define GET_PWM_VALUE(x) min(1,(1/0.2) * pow(E,((x/0.2)-5)));


void updatePWMControl() {
  
  // roll angle     
  pwmValue = GET_PWM_VALUE(ROLL_X);
  
  if(getRollAngle() > 0) {
    // left is down
    analogWrite(PWM_LEFT, middle + (pwmValue * range));
    analogWrite(PWM_RIGHT, middle - (pwmValue * range));
  } else  {
    // right is down
    analogWrite(PWM_RIGHT, middle + (pwmValue * range));
    analogWrite(PWM_LEFT, middle - (pwmValue * range));
  }

  // pitch angle 
  pwmValue = GET_PWM_VALUE(PITCH_X);
  
  if(getPitchAngle() > 0) {
    // nose is down
    analogWrite(PWM_FRONT, middle + (pwmValue * range));
    analogWrite(PWM_BACK, middle - (pwmValue * range));
  } else  {
    // back is down
    analogWrite(PWM_BACK, middle + (pwmValue * range));
    analogWrite(PWM_FRONT, middle - (pwmValue * range));
  }
}


