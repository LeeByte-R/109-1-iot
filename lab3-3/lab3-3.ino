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
PubSubClient pubsub(client);
int counter = 0;
LDHT dht(DHTPIN,DHTTYPE);
float tempC=0.0,Humi=0.0;

char sub_topic[] = "mcs/D2vZAjLs/tvfT0Nnv4iS0VD8P/myswitch";
int controlPin = 10; 
int buttonPin = 6; 
long lastMsg = 0;
char msg[50];
int value = 0;

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

void callback(char* topic, byte* payload, unsigned int length) {   //MQTT sub
  Serial.print("Input Message arrived [");
  Serial.print(sub_topic);
  Serial.print("] ");
  for (int i=0; i < length; i++)
    Serial.print(payload[i]);
  if((char)payload[length-1] == '1'){
    digitalWrite(controlPin, HIGH); 
    Serial.println("Relay is HIGH");
  }
  if((char)payload[length-1] == '0'){
    digitalWrite(controlPin, LOW); 
    Serial.println("Relay is LOW");
  }
  Serial.println();
}

void reconnect()
{
  // Loop until we're reconnected
  while (!pubsub.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (pubsub.connect(MQTT_CLIENT_ID))
    {
      pubsub.subscribe(sub_topic);
      Serial.println("connected");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(pubsub.state());
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
  pinMode(controlPin, OUTPUT);
    
  printWifiStatus();
  pubsub.setCallback(callback);
  pubsub.setServer(MQTT_SERVER_IP, MQTT_SERVER_PORT);
  delay(1500);
}

void loop() {
  if (!pubsub.connected()) {
    reconnect();
  }
  else {
    if(dht.read())
    {
        tempC = dht.readTemperature();
        Humi = dht.readHumidity();

        if(tempC>25.0){
            digitalWrite(controlPin, HIGH); 
            Serial.println("temperature > 25 Relay is HIGH");
        }else{
            digitalWrite(controlPin, LOW); 
            Serial.println("temperature <= 25 Relay is LOW");
        }
        
        char str[80];
        Serial.println("------------------------------");
        Serial.print("Temperature Celcius = ");
        Serial.print(tempC);
        Serial.println("C");
        sprintf(str, ",tmp,%f", tempC);
        pubsub.publish(MQTT_TOPIC, str);
        
        Serial.print("Humidity = ");
        Serial.print(Humi);
        Serial.println("%");
        sprintf(str, ",hum,%f", Humi);
        pubsub.publish(MQTT_TOPIC, str);
    } 
    delay(100);
  }
  
  pubsub.loop();
  delay(1000);
}
