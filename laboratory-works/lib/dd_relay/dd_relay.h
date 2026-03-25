#ifndef DD_RELAY_H
#define DD_RELAY_H

#include <Arduino.h>

class Relay {
public:
    Relay(int pin, bool normallyClosed = true);
    void begin();
    void turnOn();
    void turnOff();
    void setState(bool on);
    void toggle();
    bool isOn();
private:
    int pin;
    bool normallyClosed;
    bool stateOn;
};

#endif
