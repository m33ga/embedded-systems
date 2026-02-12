#include "dd_led.h"


Led::Led(int pin, bool activeLow = false) {
    this->pin = pin;
    stateOn = false;
    this->activeLow = activeLow; // some LEDs turn on when the pin is LOW (builtin LED on esp8266)
    this->begin();
}

void Led::begin() {
    pinMode(pin, OUTPUT);
    off();
}

void Led::on() {
    stateOn = true;
    if (activeLow) {
        digitalWrite(pin, LOW);
    } else {
        digitalWrite(pin, HIGH);
    }
}

void Led::off() {
    stateOn = false;
    if (activeLow) {
        digitalWrite(pin, HIGH);
    } else {
        digitalWrite(pin, LOW);
    }
}

void Led::toggle() {
    if (this->isOn()) {
        off();
    } else {
        on();
    }
}

bool Led::isOn() {
    return stateOn;
}
