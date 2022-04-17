#include <LWiFi.h> 
#include "MCS.h" 
char _lwifi_ssid[] ="ABC";
char _lwifi_pass[] ="123456789";
MCSDevice mcs("DJOzaAbT", "l0lnVDJT0QCe4VPv");
MCSControllerOnOff MySwitch("myswitch");
void setup() { 
  Serial.begin(9600); 
  mcs.addChannel(MySwitch); 
  Serial.println("Wi-Fi Start Connecting..."); 
  while (WiFi.begin(_lwifi_ssid, _lwifi_pass) != WL_CONNECTED) { delay(1000); } 
  Serial.println("Wi-Fi Connect Success"); 
  while(!mcs.connected()) { mcs.connect(); } 
  Serial.println("MCS Connect Success"); 
  Serial.begin(115200); 
  pinMode(LED_BUILTIN, OUTPUT); 
} 
void loop() { 
  while (!mcs.connected()) { 
    mcs.connect(); 
    if (mcs.connected()) { Serial.println("MCS Reconnect"); } 
  } 
  mcs.process(100); 
  if (MySwitch.updated()) { 
    Serial.print("Channel Update :"); 
    Serial.println(MySwitch.value()); 
    if (MySwitch.value()) { 
      digitalWrite(LED_BUILTIN, HIGH); 
    } else { 
      digitalWrite(LED_BUILTIN, LOW); 
    } } 
  delay(1000); 
}
