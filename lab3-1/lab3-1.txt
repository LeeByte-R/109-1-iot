#include <LWiFi.h>
#include <PubSubClient.h>

#include "LDHT.h"
#define DHTPIN 2          
#define DHTTYPE DHT22     

#define WIFI_SSID "ABC"
#define WIFI_PASSWORD "123456789"

#define MQTT_SERVER_IP "mqtt.mcs.mediatek.com"
#define MQTT_SERVER_PORT 1883
#define MQTT_TOPIC "mcs/D2vZAjLs/tvfT0Nnv4iS0VD8P"
#define MQTT_CLIENT_ID "linklt7697_acs106119"

int status = WL_IDLE_STATUS;
WiFiClient client;
PubSubClient upload(client);
int counter = 0;
LDHT dht(DHTPIN,DHTTYPE);
float tempC=0.0,Humi=0.0;

void printWifiStatus()
{
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}
void reconnect()
{
  // Loop until we're reconnected
  while (!upload.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (upload.connect(MQTT_CLIENT_ID))
    {
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(upload.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup()
{
  Serial.begin(9600);
  dht.begin();
  // attempt to connect to Wifi network:
  while (status != WL_CONNECTED) {
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(WIFI_SSID);
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  }
  Serial.println("Connected to wifi");
  printWifiStatus();

  upload.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
  delay(1500);
}

void loop() {
  if (!upload.connected()) {
    reconnect();
  }
  else {
    if(dht.read())
    {
        tempC = dht.readTemperature();
        Humi = dht.readHumidity();
        char str[80];
        Serial.println("------------------------------");
        Serial.print("Temperature Celcius = ");
        Serial.print(tempC);
        Serial.println("C");
        sprintf(str, ",tmp,%f", tempC);
        upload.publish(MQTT_TOPIC, str);
        
        Serial.print("Humidity = ");
        Serial.print(Humi);
        Serial.println("%");
        sprintf(str, ",hum,%f", Humi);
        upload.publish(MQTT_TOPIC, str);
    } 
    delay(100);
  }
  upload.loop();
  delay(1000);
}
