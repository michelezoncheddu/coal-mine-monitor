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
#define BOARD_ID              1
#define VOLTAGE_RESOLUTION    5
#define ADC_BIT_RESOLUTION    10 // For Arduino UNO/MEGA/NANO
#define R0_MQ9                6
#define R0_MQ135              40

const int  QoS                 = 1;
const char broker[]            = "lamponepi.duckdns.org";
const char board_info_topic[]  = "mine/boards";
const char sensor_data_topic[] = "mine/sensor_data";
const char subscribe_topic[]   = "mine/sensor_data";

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
  
  connect();
  subscribe(subscribe_topic, QoS);
  
  byte bssid[6];
  WiFi.BSSID(bssid);
  
  char publishString[64 + 1];
  sprintf(publishString, "{\"board_id\":%d,\"access_point\":\"%x:%x:%x:%x:%x:%x\"}",
      BOARD_ID, bssid[5], bssid[4], bssid[3], bssid[2], bssid[1], bssid[0]);
  publish(board_info_topic, publishString);
}

void loop() {
  byte temperature=0, humidity=0;
  int CH4=0, CO=0, CO2=0, toluene=0, NH4=0, acetone=0;
  
  readTemperature(temperature, humidity);
  readMQ9(CH4, CO);
  readMQ135(CO, CO2, toluene, NH4, acetone);

  char publishString[200 + 1];
  sprintf(publishString, "{\"board_id\":%d,\"data\":{\"methane\":%d,\"carbon_monoxide\":%d,\"carbon_dioxide\":%d,"
      "\"toluene\":%d,\"acetone\":%d,\"temperature\":%d,\"humidity\":%d}}",
      BOARD_ID, CH4, CO, CO2, toluene, acetone, temperature, humidity);
  publish(sensor_data_topic, publishString);
  
  if (!mqttClient.connected()) {
    connect();
    subscribe(subscribe_topic, QoS);
  }
  mqttClient.loop();

  delay(1000);
}
