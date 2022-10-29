#ifndef MetalDetector_H
#define MetalDetector_H
#include "Arduino.h"

class MetalDetector
{
public: 
	//Declaration Constructor
	MetalDetector(byte capacitorPulsePin, byte capacitorPin, byte buzzerPin);

	//Declaration public member functions
	bool isMetalDetected();

private:
	//Declaration private member functions and variables

	//private functions
	void initPins(byte capacitorPulsePin, byte capacitorPin, byte buzzerPin);


	//private constants variables
	const byte NUM_PULSE = 12;  
	const byte NUM_MEASERUMENT = 255; 

	//private variables
	byte capacitor_pulse_pin;
	byte capacitor_pin;
	byte buzzer_pin;

	long int sumOfSum; 
	long int skipp;    
	long int difference;    
	long int flash_period; 
	long unsigned int previous_flash; 
};

#endif