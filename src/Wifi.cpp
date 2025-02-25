#include "Wifi.h"

IPAddress ip(192, 168, 4, 2);
IPAddress gateway(192, 168, 4, 1);
IPAddress subnet(255, 255, 255, 0);

const char *Ssid = "Rapunzel2";
const char *Password = "rapunzel";

Wifi::Wifi(): dnsServer()
{
}

void Wifi::setup()
{
  WiFi.config(ip, gateway, subnet);
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
