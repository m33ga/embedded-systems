#ifndef DD_HEATER_H
#define DD_HEATER_H

#include <stdint.h>

class Heater {
public:
    Heater(uint8_t pwmPin);

    void setDuty(uint8_t duty);
    void off();
    uint8_t getDuty() const;

private:
    uint8_t pin;
    uint8_t currentDuty;
};

#endif
