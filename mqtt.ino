// QoS: 0
void publish(const char* topic, const char* data) {
  if (mqttClient.publish(topic, data))
    Serial.println("Published");
  else
    Serial.println("Not published");
}

// Max QoS: 1
void subscribe(const char* topic, int QoS) {
  if (mqttClient.subscribe(topic, QoS))
    Serial.println("Subscribed");
  else
    Serial.println("Not subscribed");
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

void connect(char * client_name) {
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqttClient.connect(client_name)) {
      Serial.println("Connected");
    } else {
      Serial.print("Failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 1 second");
      delay(1000);
    }
  }
}
