#include <SoftwareSerial.h>
#include <string.h>
#include "LDHT.h"

#define DHTPIN 2          
#define DHTTYPE DHT22     

LDHT dht(DHTPIN,DHTTYPE);

float tempC=0.0,Humi=0.0;
char str[80];
char code[20];
//Create software serial object to communicate with BC20
SoftwareSerial mySerial(3, 4); //BC20 Tx & Rx is connected to LinkIt #3 & #4

void setup(){
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  dht.begin();
  
  Serial.print(DHTTYPE);
  Serial.println(" test!");
  //Begin serial communication with LinkIt 7697 and BC20
  mySerial.begin(9600);

  Serial.println("Initializing...");
  delay(1000);

  mySerial.println("AT"); //Once the handshake test is successful, it will back to OK
  updateSerial();
  delay(2000); 
  mySerial.println("AT+QSCLK=0"); 
  updateSerial();
  delay(2000);   
  mySerial.println("AT+CSQ"); 
  updateSerial();
  delay(2000);   
  mySerial.println("AT+CGATT=0"); 
  updateSerial();
  delay(2000);   
  mySerial.println("AT+CFUN=0"); 
  updateSerial();
  delay(2000);   
  mySerial.println("AT+CGDCONT=1,\"IPV4V6\",,,0,0,,,,,0,0"); 
  updateSerial();
  delay(2000);   
  mySerial.println("AT+CFUN=1"); 
  updateSerial();
  delay(5000);
  mySerial.println("AT+CGATT=1"); 
  updateSerial();
  delay(5000);
  mySerial.println("AT+QIOPEN=1,0,\"UDP\",210.240.193.178,12345,1234,0"); 
  updateSerial();
  delay(5000); 
  mySerial.println("AT+QICFG=\"dataformat\",1,1"); 
  updateSerial();
  delay(5000);
}

void convert(float a){
    memset(code, 0, sizeof(code));
    char tmp[10];
    sprintf(tmp, "%.1f", a);
    size_t len = strlen(tmp);
    char hex[3];
    for(int i=0;i<len;i++){
        sprintf(hex, "%x", tmp[i]);
        strcat(code, hex);
    }
}

void loop(){
    if(dht.read())
    {
        tempC = dht.readTemperature();
        Humi = dht.readHumidity();

        Serial.println("------------------------------");
        Serial.print("Temperature Celcius = ");
        Serial.print(tempC);
        Serial.println("C");

        Serial.print("Humidity = ");
        Serial.print(Humi);
        Serial.println("%");
        convert(tempC);
        sprintf(str, "AT+QISEND=0,%d,%s", strlen(code)/2, code);
        mySerial.println(str); 
        updateSerial();

        convert(Humi);
        sprintf(str, "AT+QISEND=0,%d,%s", strlen(code)/2, code);
        mySerial.println(str); 
        updateSerial();
        delay(30000);
    }
}



void updateSerial()
{
  delay(1000);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}
