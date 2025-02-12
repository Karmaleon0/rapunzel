#include <Arduino.h>
#include "OTAHandler.h"
#include "Wifi.h"
#include "Schluesseldienst.cpp"

OTAHandler otaHandler;
Wifi wifi;

void setupschluesseldienst() {
  Serial.begin(115200);

  // LED konfigurieren
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  // Lichtschranke konfigurieren
  pinMode(lichtschrankePin, INPUT);

  // WiFi konfigurieren
  Serial.println("Access Point wird erstellt...");
  if (WiFi.beginAP(ssid, pass) != WL_AP_LISTENING) {
    Serial.println("Fehler beim Erstellen des Access Points!");
    while (true);
  }
  Server.begin();
  Serial.println("Access Point erfolgreich erstellt.");
  Serial.print("IP-Adresse: ");
  Serial.println(WiFi.localIP());
}

void setup() {
  otaHandler.setup();
  wifi.setup();
  setupschluesseldienst();
  
}

void loop() {
  otaHandler.loop();
  wifi.loop();
  Schluesseldienst();
}
