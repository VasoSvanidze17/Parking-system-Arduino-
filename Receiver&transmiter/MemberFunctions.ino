int checkDistance()
{
  // Clears the trigPin condition
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin HIGHACTIVE) for 10 microseconds
  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(echo_pin, HIGH);
  // Calculating the distance
  int distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  
  Serial.print(F("Distance: "));
  Serial.print(distance);
  Serial.println(F(" cm"));
  Serial.println();

  return distance;
}

String enterPasscode()
{
  Serial.print(F("Enter passcode: "));
  String passcode = "";

  for(int i = 0; i < PASSCODE_SIZE; i++)
  {
    char enteredBit = '-';

    while(enteredBit == '-')
    {
      enteredBit = isButtonClicked();
    }

    delay(250);
    passcode += enteredBit;
    Serial.print(enteredBit);
  }
  
  return passcode;
}

char isButtonClicked()
{
  Wire.requestFrom(8, 1);
  if (Wire.available()) 
  { 
    char c = Wire.read(); 

    if((c >= '0' && c <= '9') || c == '*' || c == '#')
    {
      return c;
    }
  }

  delay(500);
  
  if(digitalRead(button1_pin) == HIGH)
  {
    return '1';
  }
  else if(digitalRead(button0_pin) == HIGH)
  {
    return '0';
  }

  return '-';
}

void sendPasscodeToServer(String passcode)
{
  Transmitter.openWritingPipe(address);
  Transmitter.setPALevel(RF24_PA_MIN);
  Transmitter.stopListening();

  char text[passcode.length()];
  for(byte i = 0; i < passcode.length(); i++)
  {
    text[i] = passcode[i];
  }

  Transmitter.write(&text, sizeof(text));
  delay(500);
  Serial.println(F("Sent data"));
  Serial.println();
}

bool isCorrectPasscode(String passcode)
{
  //for testing
  // if(passcode == "1010")
  // {
  //   return true;
  // }
  // else
  // {
  //   return false;
  // }

  Transmitter.openReadingPipe(0, address);
  Transmitter.setPALevel(RF24_PA_MIN);
  Transmitter.startListening();
  bool answer = false;

  long timeStamp = millis();
  long timeStamp1 = timeStamp;
  bool isConnected = false;

  while(millis() - timeStamp <= CONNECTION_TIME * MILLISECONDS_IN_SECOND)
  {
    if(Transmitter.available())
    {
      Transmitter.read(&answer, sizeof(answer));
      isConnected = true;
      break;
    }

    if(millis() - timeStamp1 >= MILLISECONDS_IN_SECOND / 4)
    {
      timeStamp1 = millis();
      Serial.print(F("."));
    }
  }

  Serial.println();

  if(!isConnected)
  {
    Serial.println(F("Server connection fail, enter passcode again"));
    Serial.println();
    return false;
  }

  if(answer)
  {
    Serial.println(F("Answer: passcode is correct"));
    return true;
  }

  Serial.println(F("Answer: passcode is wrong"));
  Serial.println();
  
  return false;
}

void openServo()
{
  servo.write(180);
  delay(100);
  isServoOpen = true;
  delay(250);
  while(checkDistance() <= DISTANCE_OBJECT){}
  isServoOpen = false;
  servo.write(0);
  delay(15);
}

void exit()
{
  Serial.println(F("Interrupt"));
  if(isServoOpen == false)
  {
    Serial.println(F("Open servo true"));
    isServoOpen = true;
  }
}