#include "ServoController.h"

ServoController::ServoController(int servoPin, int stopAngle)
  : _servoPin(servoPin), _stopAngle(stopAngle) {
  _task.active = false;
  _task.cooling = false;
}

void ServoController::setup() {
  pinMode(_servoPin, OUTPUT);
}

void ServoController::loop() {
  if (_task.active) {
    if (!_task.cooling) {
      if (millis() - _task.startTime >= _task.duration) {
        _servo.write(_stopAngle);
        _task.cooling = true;
        _task.cooldownStartTime = millis();
      }
    }
    else {
      if (millis() - _task.cooldownStartTime >= 200) {  // 200 ms Kühlphase
        _servo.detach();
        _task.active = false;
        _task.cooling = false;
        Serial.println("Servo move completed.");
      }
    }
  }
}

void ServoController::startTask(int targetAngle, unsigned long duration) {
  if (!_task.active) {
    _servo.attach(_servoPin);
    _servo.write(targetAngle);
    _task.active = true;
    _task.targetAngle = targetAngle;
    _task.duration = duration;
    _task.startTime = millis();
    _task.cooling = false;
    Serial.print("Servo move started: angle=");
    Serial.print(targetAngle);
    Serial.print(", duration=");
    Serial.println(duration);
  }
}

bool ServoController::isBusy() const {
  return _task.active;
}