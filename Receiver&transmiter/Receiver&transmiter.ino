//included libraries
#include "MetalDetector.h"
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
#include <Wire.h>

//analog pins
#define capasitor_pulse_pin A0
#define capacitor_pin A1
#define buzzer_pin A2

//Todo:
// #define lcd_sda A4
// #define lcd_scl A5

//digital pins
#define button_exit_pin 2
#define echo_pin 3
#define trig_pin 4
#define touch_pin 5
#define servo_pin 6
#define button0_pin 7
#define button1_pin 8
#define nrf24_ce 9
#define nrf24_csn 10
#define nrf24_mosi 11
#define nrf24_miso 12
#define nrf24_sck 13

//prototype of functions
int checkDistance();
String enterPasscode();
char isButtonClicked();
void sendPasscodeToServer(String passcode);
bool isCorrectPasscode(String passcode);
void openServo();
void exit();

//constants variables
const byte address[6] = "00001";
const byte PASSCODE_SIZE = 4;
const byte ATTEMPTS = 3;
const byte DISTANCE_OBJECT = 10;
const byte PARKING_SPACES = 8;
const byte CONNECTION_TIME = 10;
const int MILLISECONDS_IN_SECOND = 1000;

//global variables
byte attempts;
bool isServoOpen;
byte spaces;

//declaration of constructors
MetalDetector MetalDetector(capasitor_pulse_pin, capacitor_pin, buzzer_pin);
RF24 Transmitter(9, 10); // CE, CSN
Servo servo;

void setup()
{
  Serial.begin(9600);
  
  attempts = ATTEMPTS;
  isServoOpen = false;
  spaces = PARKING_SPACES;
  Transmitter.begin();
  servo.attach(servo_pin);
  pinMode(trig_pin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echo_pin, INPUT); // Sets the echoPin as an INPUT
  pinMode(button_exit_pin, INPUT);
  attachInterrupt(digitalPinToInterrupt(button_exit_pin), exit, FALLING);
  Wire.begin(); 
}

void loop() 
{ 
  if(checkDistance() <= DISTANCE_OBJECT && spaces > 0)
  {
    if(MetalDetector.isMetalDetected())
    {
      Serial.println(F("Metal is detected"));
      MetalDetector.turnBuzzer(false);
      //MetalDetector.settingValues();
      
      while(true)
      {
        if(attempts > 0)
        {
          String passcode = enterPasscode();
          
          Serial.println(); 
          sendPasscodeToServer(passcode);
          
          Serial.print(F("Waiting server answer ..."));
          if(isCorrectPasscode(passcode))
          {
            spaces--;
            attempts = ATTEMPTS;
            openServo();
            break;
          }
          else
          {
            attempts--;
          }
        }
        else
        {
          break;
        }
      }
    }
  }
  else
  {
    Serial.println(F("Distance > 10"));

    if(isServoOpen == true || digitalRead(touch_pin) == 1)
    {
      if(spaces < PARKING_SPACES) spaces++;
      openServo();
    }

    isServoOpen = false;
  }
}