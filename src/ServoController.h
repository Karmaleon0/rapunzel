#ifndef SERVOCONTROLLER_H
#define SERVOCONTROLLER_H

#include <Arduino.h>
#include <Servo.h>

class ServoController
{
public:
    ServoController(int servoPin, int stopAngle = 90);

    void setup();

    void loop();

    void startTask(int targetAngle, unsigned long duration);

    bool isBusy() const;

private:
    int _servoPin;
    int _stopAngle;
    Servo _servo;

    struct Task
    {
        bool active;
        int targetAngle;
        unsigned long duration;
        unsigned long startTime;
        bool cooling;
        unsigned long cooldownStartTime;
    } _task;
};

#endif