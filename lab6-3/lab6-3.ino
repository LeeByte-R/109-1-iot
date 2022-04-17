
#include <SoftwareSerial.h>
#include <string.h>
#include <Grove_LED_Bar.h>
Grove_LED_Bar bar(5, 4, 0, LED_BAR_10);

char receive[10];
//Create software serial object to communicate with BC20
SoftwareSerial mySerial(3, 4); //BC20 Tx & Rx is connected to LinkIt #3 & #4

void setup(){
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  
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
  mySerial.println("AT+QMTOPEN=0,\"210.240.193.178\",1883"); 
  updateSerial();
  delay(5000); 
  mySerial.println("AT+QMTCONN=0,\"BC20\""); 
  updateSerial();
  delay(5000);
}

void loop(){
  mySerial.println("AT+QMTSUB=0,1,\"/public/ntcu/bc20\",1");
  if(strcmp(receive,"1") != 0){
    bar.setLevel(1);
  }else if((strcmp(receive,"10") != 0)){
    bar.setLevel(10);
  }
  updateSerial();
  delay(30000);
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
    receive = mySerial.read();
    Serial.write(receive);//Forward what Software Serial received to Serial Port
  }
}
