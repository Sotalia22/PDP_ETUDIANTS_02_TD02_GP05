#define BLYNK_TEMPLATE_ID "TMPL5YP_yo29m"
#define BLYNK_TEMPLATE_NAME "TP2NounaLebretonRankovic"
#define BLYNK_AUTH_TOKEN "x3wFY_9edsvr5b-SYKtpJalhreED_CJv"
#define BLYNK_PRINT Serial

#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <WiFiClient.h>

#include <BlynkSimpleEsp32.h>

#define SENSOR 33
#define LED 26
#define DHTTYPE DHT11

DHT_Unified dht(SENSOR, DHTTYPE);

char ssid[] = "Suzy";
char pass[] = "12345!;)";

BLYNK_WRITE(V2)
{
  int pinValue = param.asInt(); 
  Serial.print("Received value from Blynk: ");
  Serial.println(pinValue);
  digitalWrite(LED,pinValue);
  delay(1000);
}

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);

  Serial.begin(9600);
  delay(100);

  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Blynk.run();
  Blynk.syncVirtual(V2);

  dht.begin();

  sensors_event_t event;

  // Température
  float temp_measure = -999.0;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  } else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
    temp_measure = event.temperature;
  }

  // Humidité
  float relative_humidity_measure = -999.0;
  dht.humidity().getEvent(&event);
  if (isnan(event.relative_humidity)) {
    Serial.println(F("Error reading humidity!"));
  } else {
    Serial.print(F("Humidity: "));
    Serial.print(event.relative_humidity);
    Serial.println(F("%"));
    relative_humidity_measure = event.relative_humidity;
  }

  // Envoi à Blynk
  Blynk.virtualWrite(V1, temp_measure); 
  Blynk.virtualWrite(V0, relative_humidity_measure); 

  Serial.println("Data sent to Blynk");

  Serial.println("Going to sleep for 5 seconds...");
  delay(100);
  ESP.deepSleep(5e6);
}

void loop() {
}
