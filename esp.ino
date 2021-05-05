void sendATCommand(const String command) {
  Serial.println("Sending an AT command...");
  esp.println(command);
  delay(30);
  while (esp.available()) {
    String inData = esp.readStringUntil('\n');
    Serial.println("Got reponse from ESP8266: " + inData);
  }
}
