#include "MetalDetector.h"

MetalDetector::MetalDetector(byte capacitorPulsePin, byte capacitorPin, byte buzzerPin)
{
	initPins(capacitorPulsePin, capacitorPin, buzzerPin);
  	
	sumOfSum = 0; 
	skipp = 0;    
	difference = 0;    
	flash_period = 0; 
	previous_flash = 0; 
}

/*
	this function returns true is detector detects
	metal and returns false metal isn't detected
*/
bool MetalDetector::isMetalDetected()
{
	int minimalValue = 2000;
	int maximumValue = 0;

	long unsigned int sum = 0;
	byte buzzerState;

	byte i = NUM_MEASERUMENT;
	while(i > 0)
	{
		i--;

		pinMode(capacitor_pin, OUTPUT);
	    digitalWrite(capacitor_pin, LOW);
	    delayMicroseconds(20);
	    pinMode(capacitor_pin, INPUT);

	    byte j = NUM_PULSE;
	    while(j > 0)
	    {
	    	j--;
	    	digitalWrite(capacitor_pulse_pin, HIGH); //takes 3.5 microseconds
	      	delayMicroseconds(3);
	      	digitalWrite(capacitor_pulse_pin, LOW); //takes 3.5 microseconds
	      	delayMicroseconds(3);
	    }

	    int capacitorValue = analogRead(capacitor_pin);
	    minimalValue = min(capacitorValue, minimalValue);
	    maximumValue = min(capacitorValue, maximumValue);
	    sum += capacitorValue;

	    long unsigned int saveTime = millis();
	    buzzerState = 0;

	    if(saveTime < previous_flash + 12)
	    {
	    	if(difference > 0) buzzerState = 1;
	    	if(difference < 0) buzzerState = 2;
	    }

	    if(saveTime > previous_flash + flash_period)
	    {
	    	if(difference > 0) buzzerState = 1;
	    	if(difference < 0) buzzerState = 2;
	    	previous_flash = saveTime;
	    }

	    if(flash_period > 1000) buzzerState = 0;

	    if(buzzerState == 0) digitalWrite(buzzer_pin, LOW);
	    if(buzzerState == 1) digitalWrite(buzzer_pin, LOW);
	    if(buzzerState == 2) digitalWrite(buzzer_pin, HIGH);
	}

	sum -= minimalValue;
	sum -= maximumValue;

	if(sumOfSum == 0) sumOfSum = sum << 6;
	long int averageSum = (sumOfSum + 32) >> 6;
	difference = sum - averageSum;

	if(abs(difference) < averageSum >> 10)
	{
		sumOfSum = sumOfSum + sum - averageSum;
		skipp = 0;
	}
	else
	{
		skipp++;
	}

	if(skipp > 64)
	{
		sumOfSum = sum << 6;
		skipp = 0;
	}

	if(difference == 0)
	{
		flash_period = 1000000;
	}
	else
	{
		flash_period = averageSum / (2 * abs(difference));
	}

	return (buzzerState == 2);	
}


//initialization of pins
void MetalDetector::initPins(byte capacitorPulsePin, byte capacitorPin, byte buzzerPin)
{
	capacitor_pulse_pin = capacitorPulsePin;
	capacitor_pin = capacitorPin; 
	buzzer_pin = buzzerPin;

	pinMode(capacitor_pulse_pin, OUTPUT);
	digitalWrite(capacitor_pulse_pin, LOW);
	pinMode(capacitor_pin, INPUT);
	pinMode(buzzer_pin, OUTPUT);
	digitalWrite(buzzer_pin, LOW);
}