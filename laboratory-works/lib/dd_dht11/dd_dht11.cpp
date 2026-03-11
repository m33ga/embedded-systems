#include "dd_dht11.h"
#include <DHT.h>
#include <Arduino.h>

static DHT* dhtInstance = nullptr;

Dht11::Dht11(uint8_t pin) : pin(pin), rawTemperature(0), rawHumidity(0) {
    dhtInstance = new DHT(pin, DHT11);
    dhtInstance->begin();
}

bool Dht11::read() {
    // DHT11 reads can fail due to timing conflicts with FreeRTOS.
    // Retry up to 3 times with short delays between attempts.
    for (uint8_t attempt = 0; attempt < 3; attempt++) {
        float t = dhtInstance->readTemperature();
        float h = dhtInstance->readHumidity();

        if (!isnan(t) && !isnan(h)) {
            rawTemperature = (int)(t * 10);
            rawHumidity = (int)(h * 10);
            return true;
        }
        delay(100);
    }
    return false;
}

int Dht11::getRawTemperature() const {
    return rawTemperature;
}

int Dht11::getRawHumidity() const {
    return rawHumidity;
}
