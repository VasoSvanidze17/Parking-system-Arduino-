#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

void initServer();
void sendAcception(bool answer);

RF24 Server(9, 10); // CE, CSN

const byte address[6] = "00001";
String passcode = "1010";

void setup() 
{
  Serial.begin(9600);
  
  Server.begin();
  Server.openReadingPipe(0, address);
  Server.setPALevel(RF24_PA_MIN);
  Server.startListening();
}

void loop() 
{ 
  //Serial.println(Server.available());
  if (Server.available()) 
  {
    char enteredText[4] = "";
    Server.read(&enteredText, sizeof(enteredText));
    Serial.println(F("Entered data"));
    Serial.print(F("Entered text: "));

    String enteredPasscode = "";
    for(int i = 0; i < 4; i++)
    {
      enteredPasscode += enteredText[i];
    }
  
    Serial.println(enteredPasscode);

    Serial.println(F("Sending answer ..."));
    delay(1000);

    if(enteredPasscode == passcode)
    {
      Serial.println(F("Passcode is correct."));
      sendAcception(true);
    }
    else
    {
      Serial.println(F("Passcode isn't correct."));
      sendAcception(false);
    }

    Serial.println();   
  }
}

void sendAcception(bool answer)
{
  Server.openWritingPipe(address);
  Server.setPALevel(RF24_PA_MIN);
  Server.stopListening();
  Server.write(&answer, sizeof(answer));
  delay(1000);
  Server.openReadingPipe(0, address);
  Server.setPALevel(RF24_PA_MIN);
  Server.startListening();
}