#include <MQUnifiedsensor.h>
#include <PubSubClient.h>
#include <SimpleDHT.h>
#include <SoftwareSerial.h>
#include <WiFiEsp.h>

#include <Secrets.h>

int status = WL_IDLE_STATUS; // WiFi

const auto pinDHT11 = 7;
const auto pinMQ9   = A0;
const auto pinMQ135 = A1;

#define BOARD                 "Arduino UNO"
#define VOLTAGE_RESOLUTION    5
#define ADC_BIT_RESOLUTION    10 // For Arduino UNO/MEGA/NANO
#define R0_MQ9                6
#define R0_MQ135              40

const char broker[]            = "lamponepi.duckdns.org";
const char board_info_topic[]  = "mine/boards";
const char sensor_data_topic[] = "mine/sensor_data";
const char subscribe_topic[]   = "mine/#";

SimpleDHT11 DHT11(pinDHT11);
MQUnifiedsensor MQ9(BOARD, VOLTAGE_RESOLUTION, ADC_BIT_RESOLUTION, pinMQ9, "MQ-9");
MQUnifiedsensor MQ135(BOARD, VOLTAGE_RESOLUTION, ADC_BIT_RESOLUTION, pinMQ135, "MQ-135");

WiFiEspClient espClient;
PubSubClient mqttClient(espClient);
SoftwareSerial esp(50, 51); // ESP8266 TX, RX connected to these pins

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
  if (!mqttClient.connected())
    reconnect();
  mqttClient.loop();

  byte bssid[6];
  WiFi.BSSID(bssid);
  Serial.print("BSSID: ");
  Serial.print(bssid[5], HEX);
  Serial.print(":");
  Serial.print(bssid[4], HEX);
  Serial.print(":");
  Serial.print(bssid[3], HEX);
  Serial.print(":");
  Serial.print(bssid[2], HEX);
  Serial.print(":");
  Serial.print(bssid[1], HEX);
  Serial.print(":");
  Serial.println(bssid[0], HEX);
  
  char publishString[64 + 1];
  sprintf(publishString, "{\"access_point\":%x-%x-%x-%x-%x-%x}}", bssid[5], bssid[4], bssid[3], bssid[2], bssid[1], bssid[0]);
  publish(board_info_topic, publishString);
}

void loop() {
  byte temperature=0, humidity=0;
  int CH4=0, CO=0, CO2=0, toluene=0, NH4=0, acetona=0;
  
  readTemperature(temperature, humidity);
  readMQ9(CH4, CO);
  readMQ135(CO, CO2, toluene, NH4, acetona);

  char publishString[256 + 1];
  sprintf(publishString, "{\"data\":{\"methane\":%d,\"carbon_monixide\":%d,\"carbon_dioxide\":%d,"\
      "\"toluene\":%d,\"acetona\":%d,\"temperature\":%d,\"humidity\":%d}}",\
      CH4, CO, CO2, toluene, acetona, temperature, humidity);
  publish(sensor_data_topic, publishString);

  if (!mqttClient.connected())
    reconnect();
  mqttClient.loop();

  delay(1000);
}
