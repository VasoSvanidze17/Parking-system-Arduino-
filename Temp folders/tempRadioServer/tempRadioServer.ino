#include <SoftwareSerial.h>

SoftwareSerial HC12(10, 11); // HC-12 TX Pin, HC-12 RX Pin

void setup() {
  Serial.begin(9600);             // Serial port to computer
  HC12.begin(9600);               // Serial port to HC12

}

void loop() {
  if (Serial.available() > 0)  
  {
    String input = Serial.readString();        
    HC12.println(input);
  }

  if (HC12.available() > 1) 
  {      
    String input  = HC12.readString();
    Serial.println(input);      
  }

  delay(20);
}