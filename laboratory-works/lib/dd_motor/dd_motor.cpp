#include "dd_motor.h"

Motor::Motor(uint8_t pinA, uint8_t pinB)
    : pinA(pinA), pinB(pinB), currentSpeed(0) {
    begin();
}

void Motor::begin() {
    pinMode(pinA, OUTPUT);
    pinMode(pinB, OUTPUT);
    stop();
}

void Motor::setSpeed(int speed) {
    if (speed > 255) speed = 255;
    if (speed < -255) speed = -255;
    currentSpeed = speed;

    if (speed > 0) {
        analogWrite(pinA, speed);
        analogWrite(pinB, 0);
    } else if (speed < 0) {
        analogWrite(pinA, 0);
        analogWrite(pinB, -speed);
    } else {
        analogWrite(pinA, 0);
        analogWrite(pinB, 0);
    }
}

void Motor::stop() {
    currentSpeed = 0;
    analogWrite(pinA, 0);
    analogWrite(pinB, 0);
}

int Motor::getSpeed() { return currentSpeed; }

int Motor::getDirection() {
    if (currentSpeed > 0) return 1;
    if (currentSpeed < 0) return -1;
    return 0;
}
