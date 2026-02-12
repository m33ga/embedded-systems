#ifndef DD_LED_H
#define DD_LED_H

#include <Arduino.h>

class Led {
public:
    Led(int pin = LED_BUILTIN, bool activeLow = false);

    void begin();
    void on();
    void off();
    void toggle();
    bool isOn();

private:
    int pin;
    bool stateOn;
    bool activeLow;
};

#endif
