#include "KeyService.h"

KeyService::KeyService(int servoPin, int sensorPin, int ledPin)
  : _sensorPin(sensorPin),
    _ledPin(ledPin),
    _currentMode(CALIBRATION),
    _calibrated(false),
    _speedCalibrated(false),
    _roundCount(0),
    _roundTime(0),
    _speedCalibStartTime(0),
    _speedCalibStep(0),
    _lastSensorState(digitalRead(sensorPin)),
    _servoController(servoPin, 90),
    _httpHandler(80)
{
}

void KeyService::setup() {
  pinMode(_sensorPin, INPUT);
  pinMode(_ledPin, OUTPUT);

  _servoController.setup();
  _httpHandler.setup();

  _httpHandler.onLedOn([this]() { handleLedOn(); });
  _httpHandler.onLedOff([this]() { handleLedOff(); });
  _httpHandler.onKeyThrow([this]() { handleKeyThrow(); });

  Serial.println("Key Service initialized.");
}

void KeyService::loop() {
  _httpHandler.loop();
  _servoController.loop();

  switch (_currentMode) {
    case CALIBRATION:
      updateCalibration();
      break;
    case SPEED_CALIBRATION:
      updateSpeedCalibration();
      break;
    case READY:
      updateReadyState();
      break;
  }
  updateRoundCounting();
}


void KeyService::handleLedOn() {
  digitalWrite(_ledPin, HIGH);
  Serial.println("LED turned on (callback).");
}

void KeyService::handleLedOff() {
  digitalWrite(_ledPin, LOW);
  Serial.println("LED turned off (callback).");
}

void KeyService::handleKeyThrow() {
  if (_currentMode == READY && !_servoController.isBusy()) {
    unsigned long durationMs = _roundTime / 1000;
    _servoController.startTask(30, durationMs);
    _roundCount++;
    Serial.println("Key thrown (callback).");
  }
}


void KeyService::updateCalibration() {
  if (digitalRead(_sensorPin) == HIGH) {
    _calibrated = true;
    Serial.println("Calibration complete.");
    _currentMode = SPEED_CALIBRATION;
  } 
  else {
    if (!_servoController.isBusy()) {
      _servoController.startTask(30, 70);
      Serial.println("Calibrating position...");
    }
  }
}

void KeyService::updateSpeedCalibration() {
  if (_speedCalibStep == 0) {
    if (!_servoController.isBusy()) {
      _servoController.startTask(30, 200);
      _speedCalibStep = 1;
    }
  } 
  else if (_speedCalibStep == 1) {
    if (digitalRead(_sensorPin) == HIGH && !_servoController.isBusy()) {
      _speedCalibStartTime = micros();
      _servoController.startTask(30, 200);
      _speedCalibStep = 2;
    }
  } 
  else if (_speedCalibStep == 2) {
    if (digitalRead(_sensorPin) == HIGH && !_servoController.isBusy()) {
      _roundTime = micros() - _speedCalibStartTime;
      _speedCalibrated = true;
      Serial.print("Round time: ");
      Serial.print(_roundTime);
      Serial.println(" µs");
      _currentMode = READY;
    }
  }
}

void KeyService::updateReadyState() {
}

void KeyService::updateRoundCounting() {
  bool currentSensorState = digitalRead(_sensorPin);
  if (currentSensorState != _lastSensorState && currentSensorState == HIGH) {
    _roundCount++;
    Serial.print("Rounds: ");
    Serial.println(_roundCount);
  }
  _lastSensorState = currentSensorState;
}