#include <Arduino.h>
#include "OTAHandler.h"
#include "Wifi.h"

OTAHandler otaHandler;
Wifi wifi;

void setup() {
  otaHandler.setup();
  wifi.setup();
}

void loop() {
  otaHandler.loop();
  wifi.loop();
}
