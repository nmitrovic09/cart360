/* FSR testing sketch. 
 
Connect one end of FSR to power, the other end to Analog 0.
Then connect one end of a 10K resistor from Analog 0 to ground 
 
For more information see www.ladyada.net/learn/sensors/fsr.html */
 
int sensorPin = 1;     // the FSR and 10K pulldown are connected to a0
int sensorVal;     // the analog reading from the FSR resistor divider
int sensorVoltage;     // the analog reading converted to voltage
unsigned long sensorResistance;  // The voltage converted to resistance, can be very big so make "long"
unsigned long sensorConductance; 
long sensorForce;       // Finally, the resistance converted to force
 
void setup(void) {
  Serial.begin(9600);   // We'll send debugging information via the Serial monitor
}
 
void loop(void) {
  sensorVal = analogRead(sensorPin);  
  Serial.print("Analog reading = ");
  Serial.println(sensorVal);
 
  // analog voltage reading ranges from about 0 to 1023 which maps to 0V to 5V (= 5000mV)
  sensorVoltage = map(sensorVal, 0, 1023, 0, 5000);
  Serial.print("Voltage reading in mV = ");
  Serial.println(sensorVoltage);  
 
  if (sensorVoltage == 0) {
    Serial.println("No pressure");  
  } else {
    // The voltage = Vcc * R / (R + FSR) where R = 10K and Vcc = 5V
    // so FSR = ((Vcc - V) * R) / V        yay math!
    sensorResistance = 5000 - sensorVoltage;     // Voltage is in millivolts so 5V = 5000mV
    sensorResistance *= 10000;                // 10K resistor
    sensorResistance /= sensorVoltage;
    Serial.print("resistance in ohms = ");
    Serial.println(sensorResistance);
 
    sensorConductance = 1000000;           // we measure in micromhos so 
    sensorConductance /= sensorResistance;
    Serial.print("Conductance in microMhos: ");
    Serial.println(sensorConductance);
 
    // Use the two FSR guide graphs to approximate the force
    if (sensorConductance <= 1000) {
      sensorForce = sensorConductance / 80;
      Serial.print("Force in Newtons: ");
      Serial.println(sensorForce);      
    } else {
      sensorForce = sensorConductance - 1000;
      sensorForce /= 2000;
      Serial.print("Force in Newtons: ");
      Serial.println(sensorForce);            
    }
  }
  Serial.println("--------------------");
  delay(1000);
}
