#include <Arduino.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <WiFi.h>
#include <WiFiClient.h>
 
// Blynk configuration
#define BLYNK_TEMPLATE_ID "TMPL5vrQl_RXw"
#define BLYNK_TEMPLATE_NAME "TP2 Groupe 5"
#define BLYNK_AUTH_TOKEN "DMXKsX91hboZlGMApwbpMrNbc-uKvfYE"
 
#define BLYNK_PRINT Serial
#include <BlynkSimpleEsp32.h>
 
// Définition des broches
#define CAPTEUR 33
#define LED 26
#define DHTTYPE DHT11
 
// WiFi credentials
char ssid[] = "HelloWorld";
char pass[] = "testtest";
 
// Initialisation du capteur DHT
DHT_Unified dht(CAPTEUR, DHTTYPE);
 
// Fonction appelée quand on reçoit une valeur sur V2 (LED)
BLYNK_WRITE(V2)
{
  int pinValue = param.asInt(); 
  Serial.print("Received value from Blynk: ");
  Serial.println(pinValue);
  digitalWrite(LED, pinValue);
  delay(1000);
}
 
void setup() {
  // Setup pins
  pinMode(LED, OUTPUT);
  digitalWrite(LED, LOW);
  // Begin serial communication
  Serial.begin(115200);
  delay(100);
  // Connexion à Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
  Blynk.run();
 
  // Force la mise à jour du pin virtuel V2 (LED)
  Blynk.syncVirtual(V2);
  // Start listening to the DHT11
  dht.begin();
  sensors_event_t event;
 
  // Get temperature event and print its value
  float temp_measure = -999.0;
  dht.temperature().getEvent(&event);
  if (isnan(event.temperature)) {
    Serial.println(F("Error reading temperature!"));
  } else {
    Serial.print(F("Temperature: "));
    Serial.print(event.temperature);
    Serial.println(F("°C"));
    temp_measure = event.temperature;
    Serial.print(temp_measure);
  }
 
  // Get humidity event and print its value.
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
 
  // Send data to Blynk if measurements are valid
  if (temp_measure != -999.0 && relative_humidity_measure != -999.0) {
    Blynk.virtualWrite(V0, relative_humidity_measure);  // Envoi de l'humidité
    Blynk.virtualWrite(V1, temp_measure);  // Envoi de la température
    Blynk.run();  // Pour s'assurer que les données sont envoyées
  }
 
  Serial.println("Going to sleep for 5 seconds...");
  delay(100);
  ESP.deepSleep(5e6);
}
 
void loop() {
  // Not needed anymore, the function is kept so PlatformIO does not complain.
}