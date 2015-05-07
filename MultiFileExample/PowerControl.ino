//#define DEBUG_VOLTAGE

#define ANALOG_INPUT_PIN A0

#define MIN_VOLTAGE 7

#define VOLT_PER_STEP (3.3/1023.0)
#define RESISTOR_1_OHM 100.0
#define RESISTOR_2_OHM 100.0


float voltage = 0;

bool isPowerLevelOk() {
  // voltage U2
  voltage = analogRead(ANALOG_INPUT_PIN) * VOLT_PER_STEP;

  // get original voltage 
  voltage = voltage / (RESISTOR_2_OHM / (RESISTOR_1_OHM + RESISTOR_2_OHM));
  
  #ifdef DEBUG_VOLTAGE
    Serial.print("Voltage: "); Serial.print(voltage); 
    Serial.print(" - Power level ok = "); 
    Serial.println((voltage >= MIN_VOLTAGE));  
  #endif  
    
  return (voltage >= MIN_VOLTAGE);
}
