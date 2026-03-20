#include "dd_hcsr04.h"
#include <Arduino.h>

// Maximum distance ~4m = 4000mm, echo time ~23ms
#define ECHO_TIMEOUT_US  30000

HcSr04::HcSr04(uint8_t trig, uint8_t echo)
    : trigPin(trig), echoPin(echo), duration(0), distanceMm(0) {
    pinMode(trigPin, OUTPUT);
    pinMode(echoPin, INPUT);
    digitalWrite(trigPin, LOW);
}

bool HcSr04::read() {
    // Send 10us trigger pulse
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Measure echo pulse duration
    duration = pulseIn(echoPin, HIGH, ECHO_TIMEOUT_US);

    if (duration == 0) {
        return false;  // Timeout - no echo received
    }

    // Calculate distance: duration (us) / 5.8 = distance in mm
    // Speed of sound = 343 m/s = 0.343 mm/us
    // Round trip, so divide by 2: 0.343 / 2 = 0.1715 mm/us
    // distance = duration * 0.1715 ≈ duration / 5.83
    distanceMm = (int)(duration * 10 / 58);

    return true;
}

int HcSr04::getDistanceMm() const {
    return distanceMm;
}

int HcSr04::getDistanceCm() const {
    return distanceMm / 10;
}

unsigned long HcSr04::getRawDuration() const {
    return duration;
}
