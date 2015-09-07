

#define ANALOG_INPUT_PIN A0         // input pin for reading power value
#define MIN_VOLTAGE 7               // minimum voltage

#define VOLT_PER_STEP (3.3/1023.0)  // converts input step into a voltage value
#define RESISTOR_1_OHM 100.0        // resistor value 1 (Spannungsteiler)
#define RESISTOR_2_OHM 100.0        // resistor value 1 (Spannungsteiler)

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
