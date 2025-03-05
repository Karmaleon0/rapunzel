#include "KeyService.h"

KeyService::KeyService(int servoPin, int sensorPin, int ledPin)
  : _sensorPin(sensorPin),
    _ledPin(ledPin),
    _servoController(servoPin, sensorPin, 90),
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

  _currentMode = SHOULD_NULL;
}

void KeyService::loop() {
  _httpHandler.loop();
  _servoController.loop();

  if (_currentMode == SHOULD_NULL) {
    if (!_servoController.isBusy()) {
      _servoController.startTask(30, -1);
      _currentMode = NULLING;
    }
  }
  else if (_currentMode == NULLING) {
    if (!_servoController.isBusy()) {
      _servoController.startTask(150, 600);
      _currentMode = LOCKING;
    }
  }
}


void KeyService::handleLedOn() {
  digitalWrite(_ledPin, LOW);
  Serial.println("LED turned on (callback).");
}

void KeyService::handleLedOff() {
  digitalWrite(_ledPin, HIGH);
  Serial.println("LED turned off (callback).");
}

void KeyService::handleKeyThrow() {
  if (_currentMode == LOCKING) {
    _servoController.startTask(30, 3000);
    _currentMode = SHOULD_NULL;
  }
}
