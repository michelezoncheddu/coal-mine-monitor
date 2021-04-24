#include <MQUnifiedsensor.h>
#include <PubSubClient.h>
#include <SimpleDHT.h>
#include <SoftwareSerial.h>
#include <WiFiEsp.h>

#include <Secrets.h>

int status = WL_IDLE_STATUS; // WiFi

const auto pinDHT11 = 8;
const auto pinMQ9   = A1;
const auto pinMQ135 = A0;

#define BOARD                 "Arduino UNO"
#define VOLTAGE_RESOLUTION    5
#define ADC_BIT_RESOLUTION    10 // For Arduino UNO/MEGA/NANO
#define RATIO_MQ9_CLEAN_AIR   9.6
#define RATIO_MQ135_CLEAN_AIR 3.6
#define R0_MQ9                6
#define R0_MQ135              40

const char broker[]          = "lamponepi.duckdns.org";
const char publish_topic[]   = "mine/sensor_data";
const char subscribe_topic[] = "mine/#";

SimpleDHT11 DHT11(pinDHT11);
MQUnifiedsensor MQ9(BOARD, VOLTAGE_RESOLUTION, ADC_BIT_RESOLUTION, pinMQ9, "MQ-9");
MQUnifiedsensor MQ135(BOARD, VOLTAGE_RESOLUTION, ADC_BIT_RESOLUTION, pinMQ135, "MQ-135");

WiFiEspClient espClient;
PubSubClient mqttClient(espClient);
SoftwareSerial esp(3, 2); // ESP8266 TX, RX connected to these pins

void setup() {
  Serial.begin(9600);
  esp.begin(9600); // max 57600 for virtual serial
  WiFi.init(&esp);

  MQ9.setRegressionMethod(1);
  MQ9.init();
  MQ9.setR0(R0_MQ9);
  MQ135.setRegressionMethod(1);
  MQ135.init();
  MQ135.setR0(R0_MQ135);

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present");
    while (true) ; // don't continue
  }
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(SSID);
    status = WiFi.begin(SSID, PASS);
  }
  Serial.println("You're connected to the network");

  mqttClient.setServer(broker, 1883);
  mqttClient.setCallback(callback);

  // Here we must send the mine/boards packet
}

void loop() {
  readTemperature();
  readMQ9();
  readMQ135();
  publish("TEST");

  // DO NOT TOUCH --------------
  if (!mqttClient.connected())
    reconnect();
  mqttClient.loop();
  // DO NOT TOUCH --------------
  delay(1000);
}
