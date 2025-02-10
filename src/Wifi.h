#ifndef WIFI_H
#define WIFI_H

#include <ESP8266WiFi.h>
#include <DNSServer.h>

// Wifi
extern const char* Ssid;
extern const char* Password;

class Wifi {
public:
    Wifi();
    void setup();
    void loop();
private:
    DNSServer dnsServer;
};

#endif