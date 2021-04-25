
void publish(const char* topic, const char* data) {
  if (mqttClient.publish(topic, data))
    Serial.println("Published");
  else
    Serial.println("Not published");
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++) {
    char receivedChar = (char)payload[i];
    Serial.print(receivedChar);
  }
  Serial.println();
}

void reconnect() {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqttClient.connect("Arduino UNO")) {
      Serial.println("Connected");

      if (mqttClient.subscribe(subscribe_topic, 1)) // QoS
        Serial.println("Subscribed");

    } else {
      Serial.print("Failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
