#ifndef OTAHandler_h
#define OTAHandler_h

#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

class OTAHandler {
public:
  void setup();
  void loop();
};

#endif