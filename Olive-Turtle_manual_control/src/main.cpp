#include <Arduino.h>
#include <Wire.h>

#include "sensors/aht20.hpp"
#include "sensors/bmp280.hpp"
#include "sensors/ens160.hpp"

#include "actuators/ledPWM.hpp"

#include "utilities/loggerLib.hpp"

#define SDA_PIN 21
#define SCL_PIN 22

// LED PWM SETTINGS
#define LED_PIN 32
#define LED_CHANNEL 0
#define LED_RESOLUTION 8
#define LED_FREQUENCY 5000

#define MODULE_NAME "MAIN"

// Setup ESP32
void setup()
{
	// Init serial baud rate
	Serial.begin(115200);

	logInfo(MODULE_NAME, "Begin setup");

	// Setup sensors. In order:
	// 1. AHT20
	// 2. ENS160
	// 3. BMP280
	// 4. GPS
	Wire.begin(SDA_PIN, SCL_PIN);
	aht20Setup();
	ens160Setup(aht20GetTemperature(), aht20GetHumidity());
	bmp280Setup();

	// Setup LED
	setupLedPWM(LED_PIN, LED_CHANNEL, LED_FREQUENCY, LED_RESOLUTION);

	logInfo(MODULE_NAME, "End setup");
}

// Main loop
#define MAX_IN_CHARS 10
char in_chars[MAX_IN_CHARS];
int i = 0;

#define HELP "Available commands:\n\
	- help: print this message\n\
	- led <value>: set led brightness (0-255)\n\
	- led: get led brightness (0-255)\n\
	- aht: print AHT20 data\n\
	- ens: print ENS160 data\n\
	- bmp: print BMP280 data\n\
	- all: print all data\n"
void loop(){
	char in_char = ' ';
	while (Serial.available()) {
		in_char = Serial.read();
		if (int(in_char) != -1) {
			if (i == MAX_IN_CHARS - 1) {
				Serial.println("Max input chars reached");
				in_chars[MAX_IN_CHARS -1] = '\0';
				in_char = '\0';
				break;
			}
			if (in_char == '\n') {
				in_chars[i] = '\0';
				in_char = '\0';
				break;
			}
			Serial.print(in_char);
			in_chars[i++] = in_char;
		}
	}
	if (in_char == '\0') {
		Serial.printf("\nText Entered: %s\n", in_chars);
		if (strstr(in_chars, "led ")) {
			Serial.println("Setting led brightness");
			// Find the pointer to the first char after "led "
			char *value_str = strstr(in_chars, "led ") + 4;
			// Convert the string to int
			int value = atoi(value_str);
			// Set the led brightness
			setLedPWM(value);
			Serial.printf("Led brightness set: %d\n", value);
		} else if (strstr(in_chars, "led")) {
			Serial.printf("Led brightness: %d\n", getLedPWM());
		} else if (strstr(in_chars, "aht")) {
			aht20PrintData();
		} else if (strstr(in_chars, "ens")) {
			ens160PrintData();
		} else if (strstr(in_chars, "bmp")) {
			bmp280PrintData();
		} else if (strstr(in_chars, "all")) {
			Serial.println("Printing all data");
			Serial.printf("Led brightness: %d\n", getLedPWM());
			aht20PrintData();
			ens160PrintData();
			bmp280PrintData();
		} else if (strstr(in_chars, "help")) {
			Serial.println(HELP);
		} else {
			Serial.println("Unknown command");
		}
		in_chars[0] = '\0';
		i = 0;
	}

	delay(100);
}