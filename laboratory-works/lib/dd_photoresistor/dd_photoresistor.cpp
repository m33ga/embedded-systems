#include "dd_photoresistor.h"
#include <Arduino.h>

Photoresistor::Photoresistor(uint8_t analogPin) : pin(analogPin), rawValue(0) {
    pinMode(pin, INPUT);
}

int Photoresistor::read() {
    rawValue = analogRead(pin);
    return rawValue;
}

int Photoresistor::getPercent() const {
    // Map 0-1023 ADC to 0-100%
    // Note: With typical voltage divider, higher resistance (darker) = lower voltage
    // Invert if needed based on circuit configuration
    return map(rawValue, 0, 1023, 0, 100);
}

int Photoresistor::getRaw() const {
    return rawValue;
}
