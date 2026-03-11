#include "dd_dht11.h"
#include <DHT.h>

static DHT* dhtInstance = nullptr;

Dht11::Dht11(uint8_t pin) : pin(pin), rawTemperature(0), rawHumidity(0) {
    dhtInstance = new DHT(pin, DHT11);
    dhtInstance->begin();
}

bool Dht11::read() {
    float t = dhtInstance->readTemperature();
    float h = dhtInstance->readHumidity();

    if (isnan(t) || isnan(h)) {
        return false;
    }

    rawTemperature = (int)(t * 10);  // store as tenths of degree (e.g. 235 = 23.5°C)
    rawHumidity = (int)(h * 10);     // store as tenths of percent
    return true;
}

int Dht11::getRawTemperature() const {
    return rawTemperature;
}

int Dht11::getRawHumidity() const {
    return rawHumidity;
}
