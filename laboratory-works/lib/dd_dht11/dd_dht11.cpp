#include "dd_dht11.h"
#include <DHT.h>
#include <Arduino.h>

static DHT* dhtInstance = nullptr;

Dht11::Dht11(uint8_t pin) : pin(pin), rawTemperature(0), rawHumidity(0) {
    dhtInstance = new DHT(pin, DHT11);
    dhtInstance->begin();
}

bool Dht11::read() {
    // DHT11 protocol is timing-critical (microsecond bit detection).
    // Disable interrupts to prevent FreeRTOS tick from corrupting the read.
    noInterrupts();
    float t = dhtInstance->readTemperature();
    float h = dhtInstance->readHumidity();
    interrupts();

    if (isnan(t) || isnan(h)) {
        return false;
    }

    rawTemperature = (int)(t * 10);
    rawHumidity = (int)(h * 10);
    return true;
}

int Dht11::getRawTemperature() const {
    return rawTemperature;
}

int Dht11::getRawHumidity() const {
    return rawHumidity;
}
