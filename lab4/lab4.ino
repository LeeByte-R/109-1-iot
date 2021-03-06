#include "Ultrasonic.h"
#include <Grove_LED_Bar.h>
Ultrasonic ultrasonic(2);
Grove_LED_Bar bar(5, 4, 0, LED_BAR_10);

void setup(){
  Serial.begin(9600);
}

void loop(){
  int RangeInCentimeters;
  RangeInCentimeters = ultrasonic.MeasureInCentimeters();
  Serial.print(RangeInCentimeters);
  Serial.println(" cm");
  bar.setLevel(RangeInCentimeters/10);
  delay(500);
}
