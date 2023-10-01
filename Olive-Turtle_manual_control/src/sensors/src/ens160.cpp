/**
 * @file ens160.cpp
 * @author Benetton Alessandro (aleben98@gmail.com)
 * @brief
 * @version 0.1
 * @date 2023-09-19
 *
 * @copyright Copyright (c) 2023
 *
 */

#include <Arduino.h>

#include "../ens160.hpp"
#include "../../utilities/loggerLib.hpp"

#define MODULE_NAME "ENS160"

SparkFun_ENS160 myENS;

bool printedCompensation = false;

bool ens160Setup(float ambientTemp, float ambientHum) {
	logDebug(MODULE_NAME,"Begin setup");

	#ifndef DISABLE_ENS160
	// Init the sensor
	if( !myENS.begin() ) {
		logError(MODULE_NAME, "Could not communicate with the ENS160, check wiring.");
		return false;
	}

	logDebug(MODULE_NAME,"Begin ok!");


	// Reset the indoor air quality sensor's settings.
	if( myENS.setOperatingMode(SFE_ENS160_RESET) )
		Serial.println("Ready.");

	delay(100);

	// Device needs to be set to idle to apply any settings.
	// myENS.setOperatingMode(SFE_ENS160_IDLE);
	// Give values to Air Quality Sensor.
	logDebug(MODULE_NAME, "Setting temp", ambientTemp);
	logDebug(MODULE_NAME, "Setting hum", ambientHum);
	myENS.setTempCompensationCelsius(ambientTemp);
	myENS.setRHCompensationFloat(ambientHum);

	delay(500);

	// Set to standard operation
	// Others include SFE_ENS160_DEEP_SLEEP and SFE_ENS160_IDLE
	myENS.setOperatingMode(SFE_ENS160_STANDARD);

	int ensStatus = myENS.getFlags();
	Serial.print("Gas Sensor Status Flag (0 - Standard, 1 - Warm up, 2 - Initial Start Up): ");
	Serial.println(ensStatus);


	#else
	logWarning(MODULE_NAME, "ENS160 disabled");
	#endif

	logDebug(MODULE_NAME,"End setup");
	return true;
}

void ens160PrintData() {
	#ifndef DISABLE_ENS160
	if( myENS.checkDataStatus() )   {
		
		if (printedCompensation == false) {
			Serial.println("---------------------------");
			Serial.print("Compensation Relative Humidity (%): ");
			Serial.println(myENS.getRH());
			Serial.println("---------------------------");
			Serial.print("Compensation Temperature (Celsius): ");
			Serial.println(myENS.getTempCelsius());
			Serial.println("---------------------------");
			printedCompensation = true;
		}
		
		Serial.print("Air Quality Index (1-5) : ");
		Serial.println(myENS.getAQI());

		Serial.print("Total Volatile Organic Compounds: ");
		Serial.print(myENS.getTVOC());
		Serial.println("ppb");

		Serial.print("CO2 concentration: ");
		Serial.print(myENS.getECO2());
		Serial.println("ppm");

		Serial.print("Gas Sensor Status Flag (0 - Standard, 1 - Warm up, 2 - Initial Start Up): ");
		Serial.println(myENS.getFlags());

		Serial.println();

	} else {
		logInfo(MODULE_NAME, "No data to read");
	}
	#endif
}