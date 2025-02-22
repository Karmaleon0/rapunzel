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
    CALIBRATION,
    SPEED_CALIBRATION,
    READY
  };

  Mode _currentMode;

  int _sensorPin;
  int _ledPin;

  bool _calibrated;
  bool _speedCalibrated;
  int _roundCount;
  unsigned long _roundTime;
  unsigned long _speedCalibStartTime;
  int _speedCalibStep;

  bool _lastSensorState;

  ServoController _servoController;
  HttpHandler _httpHandler;

  void handleLedOn();
  void handleLedOff();
  void handleKeyThrow();

  void updateCalibration();
  void updateSpeedCalibration();
  void updateReadyState();
  void updateRoundCounting();
};

#endif