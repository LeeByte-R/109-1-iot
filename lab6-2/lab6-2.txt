
#include <SoftwareSerial.h>
#include <string.h>
//Create software serial object to communicate with BC20
SoftwareSerial mySerial(3, 4); //BC20 Tx & Rx is connected to LinkIt #3 & #4
char str[100];
char code[50];

int val = 0; 
int controlPin = 10; 
int buttonPin = 6; 
 



void setup(){
  //Begin serial communication with Arduino and Arduino IDE (Serial Monitor)
  Serial.begin(9600);
  pinMode(controlPin, OUTPUT); 
  
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
  mySerial.println("AT+QICFG=\"dataformat\",1,1"); 
  updateSerial();
  delay(5000);
}

void convert(int a){
    memset(code, 0, sizeof(code));
    char tmp[50];
    sprintf(tmp, "%d", a);
    size_t len = strlen(tmp);
    char hex[3];
    for(int i=0;i<len;i++){
        sprintf(hex, "%x", tmp[i]);
        strcat(code, hex);
    }
}

void loop(){
  Serial.println("-------------------------");
  mySerial.println("AT+QIOPEN=1,0,\"TCP\",210.240.193.178,54321,1234,0"); 
  updateSerial();
  delay(5000);
  int light = analogRead(A0);
  Serial.print("light: ");
  Serial.println(light);
  convert(light);
  sprintf(str, "AT+QISEND=0,%d,%s", strlen(code)/2, code);
  mySerial.println(str); 
  updateSerial();
  
  if (light>=500) 
  { 
      Serial.println("relay high");
      digitalWrite(controlPin, HIGH); 

  } 
  else 
  { 
      Serial.println("relay low");
      digitalWrite(controlPin, LOW); 

  } 
  
  delay(10000);
}
void updateSerial()
{
  delay(500);
  while (Serial.available()) 
  {
    mySerial.write(Serial.read());//Forward what Serial received to Software Serial Port
  }
  while(mySerial.available()) 
  {
    Serial.write(mySerial.read());//Forward what Software Serial received to Serial Port
  }
}
