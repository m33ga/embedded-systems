#include "dd_relay.h"

Relay::Relay(int pin, bool normallyClosed)
    : pin(pin), normallyClosed(normallyClosed), stateOn(false) {
    begin();
}

void Relay::begin() {
    pinMode(pin, OUTPUT);
    turnOff();
}

void Relay::turnOn() {
    stateOn = true;
    // NC + active-LOW module: HIGH = not energized = NC closed = load ON
    // NO + active-LOW module: LOW = energized = NO closed = load ON
    digitalWrite(pin, normallyClosed ? HIGH : LOW);
}

void Relay::turnOff() {
    stateOn = false;
    // NC + active-LOW module: LOW = energized = NC open = load OFF
    // NO + active-LOW module: HIGH = not energized = NO open = load OFF
    digitalWrite(pin, normallyClosed ? LOW : HIGH);
}

void Relay::setState(bool on) {
    if (on) turnOn(); else turnOff();
}

void Relay::toggle() {
    if (isOn()) turnOff(); else turnOn();
}

bool Relay::isOn() { return stateOn; }
