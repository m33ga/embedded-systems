#ifndef DD_HCSR04_H
#define DD_HCSR04_H

#include <stdint.h>

class HcSr04 {
public:
    HcSr04(uint8_t trigPin, uint8_t echoPin);

    // Trigger measurement and read distance
    // Returns true on successful read, false on timeout
    bool read();

    // Get distance from last successful read (in mm for precision)
    int getDistanceMm() const;

    // Get distance in cm
    int getDistanceCm() const;

    // Get raw echo duration in microseconds
    unsigned long getRawDuration() const;

private:
    uint8_t trigPin;
    uint8_t echoPin;
    unsigned long duration;
    int distanceMm;
};

#endif
