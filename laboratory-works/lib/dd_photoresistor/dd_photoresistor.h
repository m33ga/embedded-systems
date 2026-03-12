#ifndef DD_PHOTORESISTOR_H
#define DD_PHOTORESISTOR_H

#include <stdint.h>

class Photoresistor {
public:
    Photoresistor(uint8_t analogPin);

    // Read analog value from sensor
    // Returns ADC value (0-1023 on Arduino)
    int read();

    // Get percentage (0-100) from last read
    // Higher value = more light
    int getPercent() const;

    // Get raw ADC value from last read
    int getRaw() const;

private:
    uint8_t pin;
    int rawValue;
};

#endif
