#include <Arduino.h>
#include <Wire.h>

#include "sensors/aht20.hpp"
#include "sensors/bmp280.hpp"
#include "sensors/ens160.hpp"


#define SDA_PIN 21
#define SCL_PIN 22

// Counter for sent messages interval.
int counter = 0;

// Counter time
unsigned long currentTime = 0;
unsigned long sendCounter = 1;

// Setup ESP32
void setup(){
	// Init serial baud rate
	Serial.begin(115200);

	// Setup sensors. In order:
	// 1. AHT20
	// 2. ENS160
	// 3. BMP280
	// 4. GPS
	Wire.begin(SDA_PIN, SCL_PIN);
	aht20Setup();
	ens160Setup(aht20GetTemperature(), aht20GetHumidity());
	bmp280Setup();
	
}

// Main loop
void loop() {


	delay(1000);
}