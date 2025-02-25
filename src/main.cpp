#include <Arduino.h>
#include "OTAHandler.h"
#include "Wifi.h"
#include "KeyService.h"

OTAHandler otaHandler;
Wifi wifi;

#define SERVO_PIN 2 //Pin 4 but GPIO 2
#define SENSOR_PIN 14 //pin 5 but GPIO 14
KeyService keyService(SERVO_PIN, SENSOR_PIN, LED_BUILTIN);

void setup() {
  Serial.begin(115200);
  otaHandler.setup();
  wifi.setup();
  keyService.setup();
}

void loop() {
  otaHandler.loop();
  wifi.loop();
  keyService.loop();
}