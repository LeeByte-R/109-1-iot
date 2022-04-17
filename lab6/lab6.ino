
#include <SoftwareSerial.h>

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
  mySerial.println("AT+QIOPEN=1,0,\"UDP\",210.240.193.178,12345,1234,0"); 
  updateSerial();
  delay(5000); 
  mySerial.println("AT+QICFG=\"dataformat\",1,1"); 
  updateSerial();
  delay(5000);
}

void loop(){
  mySerial.println("AT+QISEND=0,3,313233"); 
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
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}
