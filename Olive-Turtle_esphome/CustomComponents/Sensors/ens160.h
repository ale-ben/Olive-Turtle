#include "esphome.h"
#include <AHT20.h>
#include <SparkFun_ENS160.h>


#define AHT_MESAUREMENT_INTERVAL_MS 2000

SparkFun_ENS160 myENS;
AHT20 aht20;

unsigned long lastMeasurement = 0;

bool printedCompensation = false;

void refreshMeasurement(){
	#ifndef DISABLE_AHT20
	if (millis()>lastMeasurement+AHT_MESAUREMENT_INTERVAL_MS) {
		while (!aht20.available()) delay(100);
		lastMeasurement = millis();
	}
	#endif
}

float aht20GetTemperature(){
	refreshMeasurement();
	return aht20.getTemperature();
}

float aht20GetHumidity(){
	refreshMeasurement();
	return aht20.getHumidity();
}

class ENS160Sensor : public PollingComponent, public Sensor {

public:
	Sensor *aqi_sensor = new Sensor();
	Sensor *tvoc_sensor = new Sensor();
	Sensor *eco2_sensor = new Sensor();
	Sensor *status_sensor = new Sensor();
	Sensor *temp_sensor = new Sensor();
	Sensor *hum_sensor = new Sensor();

	// constructor
	ENS160Sensor() : PollingComponent(10000) {} // 60s

	float get_setup_priority() const override { 
		return esphome::setup_priority::BUS;
	}

	void setup() override {
		ESP_LOGD("ENS160", "Setting up ENS160");

		if (aht20.begin() == false)  {
			ESP_LOGW("ENS-AHT20", "AHT20 not detected. Please check wiring.");
		}

		// Init the sensor
		if( !myENS.begin() ) {
			ESP_LOGE("EMS_AHT20", "Could not communicate with the ENS160, check wiring.");
			return;
		}

		// Reset the indoor air quality sensor's settings.
		myENS.setOperatingMode(SFE_ENS160_RESET);

		delay(100);

		// Device needs to be set to idle to apply any settings.
		// myENS.setOperatingMode(SFE_ENS160_IDLE);
		// Give values to Air Quality Sensor.
		myENS.setTempCompensationCelsius(aht20GetTemperature());
		myENS.setRHCompensationFloat(aht20GetHumidity());

		delay(500);

		// Set to standard operation
		// Others include SFE_ENS160_DEEP_SLEEP and SFE_ENS160_IDLE
		myENS.setOperatingMode(SFE_ENS160_STANDARD);

		ESP_LOGD("ENS160", "Setting up ENS160 done");
	}

	void update() override {
		// This will be called every "update_interval" milliseconds.
		if (myENS.checkDataStatus()) {
			aqi_sensor->publish_state(myENS.getAQI());
			tvoc_sensor->publish_state(myENS.getTVOC());
			eco2_sensor->publish_state(myENS.getECO2());
			status_sensor->publish_state(myENS.getFlags());
			temp_sensor->publish_state(aht20GetTemperature());
			hum_sensor->publish_state(aht20GetHumidity());
		} else {
			ESP_LOGW("ENS160", "No data available");
		}
	}
	
};