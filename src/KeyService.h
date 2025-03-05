#ifndef KEYSERVICE_H
#define KEYSERVICE_H

#include <Arduino.h>
#include "ServoController.h"
#include "HttpHandler.h"

class KeyService {
public:
  KeyService(int servoPin, int sensorPin, int ledPin);

  void setup();

  void loop();

private:
  enum Mode {
    SHOULD_NULL,
    NULLING,
    LOCKING,
  };

  Mode _currentMode;

  int _sensorPin;
  int _ledPin;

  ServoController _servoController;
  HttpHandler _httpHandler;

  void handleLedOn();
  void handleLedOff();
  void handleKeyThrow();
};

#endif