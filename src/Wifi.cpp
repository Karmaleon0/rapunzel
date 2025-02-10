#include "Wifi.h"

const char *Ssid = "Rapunzel";
const char *Password = "rapunzel";

Wifi::Wifi(): dnsServer()
{
}

void Wifi::setup()
{
  WiFi.softAP(Ssid, Password);
  dnsServer.start(53, "*", WiFi.softAPIP());
  Serial.print("AP IP address: ");
  // 192.168.4.1
  Serial.println(WiFi.softAPIP());
}

void Wifi::loop()
{
  dnsServer.processNextRequest();
}
