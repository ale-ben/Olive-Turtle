# 1) Monitoraggio parametri stanza
- 1 o più esp (potenzialmente divise per locale) che monitorano i parametri della stanza (temperatura, umidità, iaq, ecc..)
- Invio dei dati al server
	- Tramite MQTT
	- Tramite COAP
	- Tramite LORA?
- Storage dei dati
	- InfluxDB ( o una variante rrd?)
- Visualizzazione dei dati
	- Grafana
	- Dashboard custom?
- Notifiche
	- Impostazione soglie di allarme
- (OPZIONALE) Attuatori nella stanza
	- Riscaldamento
	- Ventilazione



#include <Arduino.h>
#include <DFRobot_ENS160.h>

#include <WiFi.h>
#include <WiFiMulti.h>

WiFiMulti WiFiMulti;


uint8_t csPin = 10;
DFRobot_ENS160_SPI ENS160(&SPI, csPin);

void setup(void) {
	Serial.begin(115200);

	// We start by connecting to a WiFi network
	WiFiMulti.addAP("Hamlet", "rldbdudv");
	Serial.println();
	Serial.println("WiFi Init\n******************************************************");
	
	while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

	Serial.println("");
	Serial.println("WiFi connected");
	Serial.println("IP address: ");
	Serial.println(WiFi.localIP());

	// Init the sensor
	while (NO_ERR != ENS160.begin()) {
		Serial.println("Communication with device failed, please check connection");
		delay(3000);
	}
	Serial.println("Begin ok!");

	ENS160.setPWRMode(ENS160_STANDARD_MODE);

	/**
	 * Users write ambient temperature and relative humidity into ENS160 for
	 * calibration and compensation of the measured gas data. ambientTemp
	 * Compensate the current ambient temperature, float type, unit: C
	 * relativeHumidity Compensate the current ambient temperature, float type,
	 * unit: %rH
	 * TODO: Implement a temperature and humidity sensor
	 */
	ENS160.setTempAndHum(/*temperature=*/ 20.0, /*humidity=*/ 50.0);
}

void loop() {
	/**
	 * Get the sensor operating status
	 * Return value: 0-Normal operation,
	 *         1-Warm-Up phase, first 3 minutes after power-on.
	 *         2-Initial Start-Up phase, first full hour of operation after
	 * initial power-on. Only once in the sensor’s lifetime. note: Note that the
	 * status will only be stored in the non-volatile memory after an initial 24h
	 * of continuous operation. If unpowered before conclusion of said period, the
	 * ENS160 will resume "Initial Start-up" mode after re-powering.
	 */
	uint8_t Status = ENS160.getENS160Status();
	Serial.print("Sensor operating status : ");
	Serial.println(Status);

	/**
	 * Get the air quality index
	 * Return value: 1-Excellent, 2-Good, 3-Moderate, 4-Poor, 5-Unhealthy
	 */
	uint8_t AQI = ENS160.getAQI();
	Serial.print("Air quality index : ");
	Serial.println(AQI);

	/**
	 * Get TVOC concentration
	 * Return value range: 0–65000, unit: ppb
	 */
	uint16_t TVOC = ENS160.getTVOC();
	Serial.print("Concentration of total volatile organic compounds : ");
	Serial.print(TVOC);
	Serial.println(" ppb");

	/**
	 * Get CO2 equivalent concentration calculated according to the detected data
	 * of VOCs and hydrogen (eCO2 – Equivalent CO2) Return value range: 400–65000,
	 * unit: ppm Five levels: Excellent(400 - 600), Good(600 - 800), Moderate(800
	 * - 1000), Poor(1000 - 1500), Unhealthy(> 1500)
	 */
	uint16_t ECO2 = ENS160.getECO2();
	Serial.print("Carbon dioxide equivalent concentration : ");
	Serial.print(ECO2);
	Serial.println(" ppm");

	Serial.println();
	delay(1000);
}