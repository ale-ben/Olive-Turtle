#include "esphome.h"
#include <AHT20.h>
#include <DFRobot_ENS160.h>


#define ENS160_ADDRESS 0x53
#define AHT_MESAUREMENT_INTERVAL_MS 2000

DFRobot_ENS160_I2C ENS160(&Wire, /*I2CAddr*/ ENS160_ADDRESS);
AHT20 aht20;

unsigned long lastMeasurement = 0;

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
	ENS160Sensor() : PollingComponent(60000) {}

	float get_setup_priority() const override { 
		return esphome::setup_priority::BUS;
	}

	void setup() override {
		ESP_LOGD("ENS160", "Setting up ENS160");
		Wire.begin();

		if (aht20.begin() == false)  {
			ESP_LOGW("ENS-AHT20", "AHT20 not detected. Please check wiring.");
		}

		ENS160.begin();
		ENS160.setPWRMode(ENS160_STANDARD_MODE);
		ENS160.setTempAndHum(aht20GetTemperature(), aht20GetHumidity());
		ESP_LOGD("ENS160", "Setting up ENS160 done");
	}

	void update() override {
		// This will be called every "update_interval" milliseconds.
		aqi_sensor->publish_state(ENS160.getAQI());
		tvoc_sensor->publish_state(ENS160.getTVOC());
		eco2_sensor->publish_state(ENS160.getECO2());
		status_sensor->publish_state(ENS160.getENS160Status());
		temp_sensor->publish_state(aht20GetTemperature());
		hum_sensor->publish_state(aht20GetHumidity());
	}
	
};