#ifndef DD_DHT11_H
#define DD_DHT11_H

#include <stdint.h>

class Dht11 {
public:
    Dht11(uint8_t pin);

    // Reads sensor; returns true on success
    bool read();

    // Raw integer values from last successful read
    int getRawTemperature() const;
    int getRawHumidity() const;

private:
    uint8_t pin;
    int rawTemperature;
    int rawHumidity;
};

#endif
