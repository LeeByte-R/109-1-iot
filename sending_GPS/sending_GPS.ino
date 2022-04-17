
void loop(void)
{
    lora.transferPacketP2PMode("Hello World!");
    SerialUSB.println("Send string.");
    delay(3000);
}

#include <TinyGPS++.h>
#include <LoRaWan.h>
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;

void setup()
{
  SerialUSB.begin(115200);
  Serial2.begin(GPSBaud);
}

void loop()
{
  // This sketch displays information every time a new sentence is correctly encoded.
  SerialUSB.begin(115200);
  lora.init();
  lora.initP2PMode(433, SF12, BW125, 8, 8, 20);
  while (Serial2.available() > 0)
    if (gps.encode(Serial2.read()))
      displayInfo();

  if (millis() > 5000 && gps.charsProcessed() < 10)
  {
    SerialUSB.println(F("No GPS detected: check wiring."));
    while(true);
  }
}

void displayInfo()
{
  SerialUSB.print(F("Location: ")); 
  if (gps.location.isValid())
  {
    lora.transferPacketP2PMode(gps.location.lat());
    lora.transferPacketP2PMode(gps.location.lng());
    SerialUSB.println("Send GPS.");
    delay(3000);
  }
  else
  {
    SerialUSB.print(F("INVALID"));
  }

  SerialUSB.println();
}
