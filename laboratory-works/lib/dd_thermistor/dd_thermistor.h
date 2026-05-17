#ifndef DD_THERMISTOR_H
#define DD_THERMISTOR_H

#include <stdint.h>

class Thermistor {
public:
    Thermistor(uint8_t analogPin, int seriesResistorOhms = 10000,
               int nominalResistanceOhms = 10000, int nominalTempC = 25,
               int bCoefficient = 3950);

    // Returns temperature in tenths of degree Celsius (e.g. 253 = 25.3C)
    int readTenths();

    int getRawADC() const;

private:
    uint8_t pin;
    int seriesR;
    int nominalR;
    int nominalT;
    int bCoeff;
    int lastADC;
};

#endif
