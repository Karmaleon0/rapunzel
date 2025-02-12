#include <Servo.h>
#include "WiFi.h"

// WiFi-Konfiguration
//const char ssid[] = "Rapunzel";
//const char pass[] = "keyplease29";

//WiFiServer server(80);

// Pins
const int servoPin = 6;
const int lichtschrankePin = 3;
const int ledPin = LED_BUILTIN;

// Servo
Servo myServo;

// Globale Variablen
bool kalibriert = false;
bool speedKalibriert = false;
bool throwKey = false;

int schritte = 0;

const unsigned long maxDauer = 60000; // Sicherheitszeitbegrenzung: 60 Sekunden

unsigned long rundeZeit = 0;
unsigned long startZeit = 0;

// Servo-Dreh Richtung und Geschwindigkeit des 360-Grad-Servos
int drosselung = 60; // Kann zwischen 90 und 1 sein
int stopp = 90;
int vorwaerts = stopp + drosselung;
int rueckwaerts = stopp - drosselung;

// Zentrale Servo-Funktion
void bewegeServo(int Richtungundgeschwindigkeit, int dauer) {
  myServo.attach(servoPin);
  myServo.write(Richtungundgeschwindigkeit);
  delay(dauer);
  myServo.write(stopp);
  delay(200);  // Abkühlzeit für den Servo
  myServo.detach();
  Serial.print("Servo bewegt: Richtungundgeschwindigkeit = ");
  Serial.print(Richtungundgeschwindigkeit);
  Serial.print(", Dauer = ");
  Serial.println(dauer);
}

void setup() {
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
  server.begin();
  Serial.println("Access Point erfolgreich erstellt.");
  Serial.print("IP-Adresse: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  handleWiFiClient();

  if (!kalibriert) {
    kalibrieren();
  } else if (!speedKalibriert) {
    kalibriereRichtungundgeschwindigkeit();
  } else if (throwKey) {
    werfeSchluessel();
  }
  roundscounting();
}

void handleWiFiClient() {
  WiFiClient client = server.available();
  if (!client) return;

  Serial.println("Neuer Client verbunden.");
  while (client.connected()) {
    if (client.available()) {
      String request = client.readStringUntil('\n');
      Serial.println(request);

      if (request.indexOf("GET /H") >= 0) {
        digitalWrite(ledPin, HIGH);
        Serial.println("LED eingeschaltet.");
      } else if (request.indexOf("GET /O") >= 0) {
        digitalWrite(ledPin, LOW);
        Serial.println("LED ausgeschaltet.");
      } else if (request.indexOf("GET /L") >= 0) {
        throwKey = true;
        Serial.println("Schlüsselwurf aktiviert.");
      }

      // HTTP-Antwort
      client.println("HTTP/1.1 200 OK");
      client.println("Content-type:text/html");
      client.println();
      client.println("<a href=\"/H\">LED an</a><br>");
      client.println("<a href=\"/O\">LED aus</a><br>");
      client.println("<a href=\"/L\">Schlüssel werfen</a><br>");
      client.println();
      break;
    }
  }
  client.stop();
  Serial.println("Client getrennt.");
}

void kalibrieren() {
  if (digitalRead(lichtschrankePin) == HIGH) {
    kalibriert = true;
    Serial.println("Kalibrierung abgeschlossen.");
  } else {
    bewegeServo(rueckwaerts, 70);  // Feine Bewegung zur Kalibrierung
    Serial.println("Kalibriere Position...");
  }
}

void kalibriereRichtungundgeschwindigkeit() {
  static int durchlaeufe = 0;

  if (durchlaeufe == 0) {
    bewegeServo(rueckwaerts, 200);
    durchlaeufe++;
  } else if (durchlaeufe == 1 && digitalRead(lichtschrankePin) == HIGH) {
    startZeit = micros();
    bewegeServo(rueckwaerts, 200);
    durchlaeufe++;
  } else if (durchlaeufe == 2 && digitalRead(lichtschrankePin) == HIGH) {
    rundeZeit = micros() - startZeit;
    speedKalibriert = true;
    Serial.print("Rundenzeit: ");
    Serial.print(rundeZeit);
    Serial.println(" µs");
  }
}

void werfeSchluessel() {
  bewegeServo(rueckwaerts, rundeZeit / 1000); // Zeit in Millisekunden
  throwKey = false;
  schritte++;
  Serial.println("Schlüssel geworfen.");
}

void roundscounting() {
  static bool lastState = LOW;
  bool currentState = digitalRead(lichtschrankePin);

  if (currentState != lastState && currentState == HIGH) {
    schritte++;
    Serial.print("Runden: ");
    Serial.println(schritte);
  }
  lastState = currentState;
}

void backToLoopCondition() {
  Serial.println("Starte Rückwärtsfahrt, um Hebel auf Nullposition zu bringen...");

  startZeit = millis();

  while (digitalRead(lichtschrankePin) != HIGH) {
    bewegeServo(rueckwaerts, 100);
    delay(50);

    if (millis() - startZeit > maxDauer) {
      Serial.println("Rückwärtsfahrt abgebrochen! Sicherheitszeit überschritten.");
      Blink();
      break;
    }
  }
  Serial.println("Hebel hat die Lichtschranke passiert. Nullposition erreicht.");
}

void Blink() {
  for (int i = 0; i < 10; i++) {
    digitalWrite(ledPin, HIGH);
    delay(100);
    digitalWrite(ledPin, LOW);
    delay(100);
  }
}
