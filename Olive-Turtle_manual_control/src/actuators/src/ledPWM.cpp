#include <Arduino.h>

#include "../ledPWM.hpp"

void setupLedPWM(int pin, int channel, int frequency, int resolution){
	// Configure LED PWM functionalitites
	ledcSetup(channel, frequency, resolution);

	// Attach the channel to the GPIO to be controlled
	ledcAttachPin(pin, channel);	
}

void setLedPWM(int value){
	ledcWrite(0, value);
}
int getLedPWM(){
	return ledcRead(0);
}