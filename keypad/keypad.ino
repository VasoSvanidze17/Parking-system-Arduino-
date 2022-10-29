#include <Keypad.h>
#include <Wire.h>

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = 
{
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {2, 3, 4, 5};
byte colPins[COLS] = {6, 7, 8};

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() 
{
  Serial.begin(9600);
  Wire.begin(8);
  Wire.onRequest(requestEvent);
}

void loop() 
{

}

void requestEvent() 
{
  char key = keypad.getKey();
   Wire.write(key);  
  // if (key) 
  // {
   
  //   // Serial.print(key);
  //   // Serial.print(" ");
  // }
}