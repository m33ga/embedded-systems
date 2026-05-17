#include "dd_thermistor.h"
#include <Arduino.h>
#include <math.h>

Thermistor::Thermistor(uint8_t analogPin, int seriesResistorOhms,
                       int nominalResistanceOhms, int nominalTempC,
                       int bCoefficient)
    : pin(analogPin), seriesR(seriesResistorOhms),
      nominalR(nominalResistanceOhms), nominalT(nominalTempC),
      bCoeff(bCoefficient), lastADC(0) {
    pinMode(pin, INPUT);
}

int Thermistor::readTenths() {
    lastADC = analogRead(pin);

    if (lastADC <= 0) lastADC = 1;
    if (lastADC >= 1023) lastADC = 1022;

    // Voltage divider: Vcc -- [seriesR] -- ADC -- [NTC] -- GND
    float resistance = (float)seriesR * (float)lastADC / (1023.0f - (float)lastADC);

    // Steinhart-Hart B-parameter equation
    float steinhart = log(resistance / (float)nominalR) / (float)bCoeff;
    steinhart += 1.0f / ((float)nominalT + 273.15f);
    float tempK = 1.0f / steinhart;
    float tempC = tempK - 273.15f;

    return (int)(tempC * 10.0f);
}

int Thermistor::getRawADC() const {
    return lastADC;
}
