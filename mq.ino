
void readMQ9() {
  Serial.println("|    LPG   |  CH4 |   CO  |");
  MQ9.update(); // Update data, the arduino will be read the voltage on the analog pin
  
  MQ9.setA(1000.5); MQ9.setB(-2.186);
  float LPG = MQ9.readSensor();
  
  MQ9.setA(4269.6); MQ9.setB(-2.648);
  float CH4 = MQ9.readSensor();
  
  MQ9.setA(599.65); MQ9.setB(-2.244);
  float CO = MQ9.readSensor();
  
  Serial.print("|    "); Serial.print(LPG);
  Serial.print("    |    "); Serial.print(CH4);
  Serial.print("    |    "); Serial.print(CO); 
  Serial.println("    |");
  delay(500);
}

void readMQ135() {
  Serial.println("|    CO   |  Alcohol |   CO2  |  Tolueno  |  NH4  |  Acetona  |");  
  MQ135.update(); // Update data, the arduino will be read the voltage on the analog pin

  MQ135.setA(605.18); MQ135.setB(-3.937);
  float CO = MQ135.readSensor();

  MQ135.setA(77.255); MQ135.setB(-3.18);
  float Alcohol = MQ135.readSensor();

  MQ135.setA(110.47); MQ135.setB(-2.862);
  float CO2 = MQ135.readSensor();

  MQ135.setA(44.947); MQ135.setB(-3.445);
  float Tolueno = MQ135.readSensor();

  MQ135.setA(102.2 ); MQ135.setB(-2.473);
  float NH4 = MQ135.readSensor();

  MQ135.setA(34.668); MQ135.setB(-3.369);
  float Acetona = MQ135.readSensor();

  Serial.print("|   "); Serial.print(CO); 
  Serial.print("   |   "); Serial.print(Alcohol); 
  Serial.print("   |   "); Serial.print(CO2); 
  Serial.print("   |   "); Serial.print(Tolueno); 
  Serial.print("   |   "); Serial.print(NH4); 
  Serial.print("   |   "); Serial.print(Acetona);
  Serial.println("   |"); 
  delay(500);
}
