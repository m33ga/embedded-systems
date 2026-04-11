#include "dd_heater.h"
#include <Arduino.h>

Heater::Heater(uint8_t pwmPin)
    : pin(pwmPin), currentDuty(0) {
    pinMode(pin, OUTPUT);
    analogWrite(pin, 0);
}

void Heater::setDuty(uint8_t duty) {
    currentDuty = duty;
    analogWrite(pin, currentDuty);
}

void Heater::off() {
    setDuty(0);
}

uint8_t Heater::getDuty() const {
    return currentDuty;
}
