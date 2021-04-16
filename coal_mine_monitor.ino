#include <WiFiEsp.h>
#include <PubSubClient.h>
#include <SoftwareSerial.h>

#include <Secrets.h>

int status = WL_IDLE_STATUS;

WiFiEspClient espClient;
PubSubClient mqttClient(espClient);
SoftwareSerial esp(3, 2); // ESP8266 TX, RX connected to these pins

void setup() {
  Serial.begin(9600);
  esp.begin(9600); // max 57600 for virtual serial
  WiFi.init(&esp);

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true) // don't continue
      ;
  }

  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(SSID);
    status = WiFi.begin(SSID, PASS);
  }

  Serial.println("You're connected to the network");

  mqttClient.setServer("192.168.1.19", 1883);
  mqttClient.setCallback(callback);
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    char receivedChar = (char)payload[i];
    Serial.print(receivedChar);
  }
  Serial.println();
}

void loop() {
  /*Serial.println("Sending an AT command...");
  esp.println("AT");
  delay(30);
  while (esp.available()) {
     String inData = esp.readStringUntil('\n');
     Serial.println("Got reponse from ESP8266: " + inData);
  }*/

  if (mqttClient.publish("fakecoalmine/board_1", "FUGGITE TUTTI, CAZZO!"))
    Serial.println("Published");
  else
    Serial.println("Not published");

  if (!mqttClient.connected()) {
    reconnect();
  }
  mqttClient.loop();
  delay(2000);
}

void reconnect() {
  // Loop until we're reconnected
  while (!mqttClient.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (mqttClient.connect("UNO")) {
      Serial.println("connected");
      
      if (mqttClient.subscribe("fakecoalmine/#", 1))
        Serial.println("Subscribed");

    } else {
      Serial.print("failed, rc=");
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}
