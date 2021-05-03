void readMQ9(int &CH4, int &CO) {
  Serial.println("|  CH4 |   CO  |");
  MQ9.update(); // Update data, the arduino will be read the voltage on the analog pin
  
  MQ9.setA(4269.6); MQ9.setB(-2.648);
  CH4 = round(MQ9.readSensor());
  
  MQ9.setA(599.65); MQ9.setB(-2.244);
  CO = round(MQ9.readSensor());
  
  Serial.print("|    "); Serial.print(CH4);
  Serial.print("    |    "); Serial.print(CO); 
  Serial.println("    |");
}

void readMQ135(int &CO, int &CO2, int &toluene, int &NH4, int &acetone) {
  Serial.println("|    CO   |   CO2  |  Toluene  |  NH4  |  Acetone  |");  
  MQ135.update(); // Update data, the arduino will be read the voltage on the analog pin

  MQ135.setA(605.18); MQ135.setB(-3.937);
  CO = round(MQ135.readSensor());

  MQ135.setA(110.47); MQ135.setB(-2.862);
  CO2 = round(MQ135.readSensor());

  MQ135.setA(44.947); MQ135.setB(-3.445);
  toluene = round(MQ135.readSensor());

  MQ135.setA(102.2); MQ135.setB(-2.473);
  NH4 = round(MQ135.readSensor());

  MQ135.setA(34.668); MQ135.setB(-3.369);
  acetone = round(MQ135.readSensor());

  Serial.print("|   "); Serial.print(CO); 
  Serial.print("   |   "); Serial.print(CO2); 
  Serial.print("   |   "); Serial.print(toluene); 
  Serial.print("   |   "); Serial.print(NH4); 
  Serial.print("   |   "); Serial.print(acetone);
  Serial.println("   |"); 
}
