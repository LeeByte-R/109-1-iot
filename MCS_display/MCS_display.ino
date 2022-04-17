#include <LWiFi.h>
#include "MCS.h"
int j;
char _lwifi_ssid[] = "ABC";
char _lwifi_pass[] = "123456789";

MCSDevice mcs("DJOzaAbT", "l0lnVDJT0QCe4VPv");
MCSDisplayInteger temp("data");
void setup() {
  Serial.begin(9600);
  mcs.addChannel(temp);
  Serial.println("Wi-Fi Start Connecting...");
  while (WiFi.begin(_lwifi_ssid, _lwifi_pass) != WL_CONNECTED){
      delay(1000); 
  }
  Serial.println("Wi-Fi Connect Success");
  while(!mcs.connected()) { mcs.connect(); }
  Serial.println("MCS Connect Success");
}

void loop() {
  while (!mcs.connected()) {
    mcs.connect();
    if (mcs.connected()) { Serial.println("MCS Reconnect"); }
  }
  temp.set(j);
  Serial.print("input value : ");
  Serial.println(j);
  j++;
  if(j==100)
    j=0;
}
