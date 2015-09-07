

/************ general includes ************/
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050_6Axis_MotionApps20.h"


/************ pin definitions ************/

#define LED_PIN 13            // led output
#define ANALOG_INPUT_PIN A0   // input pin for reading power value

#define PWM_FRONT 3  // pwm output pin
#define PWM_RIGHT 5  // pwm output pin
#define PWM_BACK 6   // pwm output pin
#define PWM_LEFT 9   // pwm output pin


/************ main ************/

#define BAUD_RATE 38400

bool flightControlReady = false; 
bool mpu6050Ready = false;

#define controllerReady \
  (flightControlReady == true && mpu6050Ready == true)


/************ pwm control ************/

uint8_t pwmRange = 16;

uint8_t pwmMiddleFront = 0;
uint8_t pwmMiddleBack = 0;
uint8_t pwmMiddleLeft = 0;
uint8_t pwmMiddleRight = 0;

/************ power control ************/

         

