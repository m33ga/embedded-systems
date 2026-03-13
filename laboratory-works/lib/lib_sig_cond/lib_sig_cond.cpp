#include "lib_sig_cond.h"

// --- Saturation ---

int sigCondSaturate(int value, int minVal, int maxVal) {
    if (value < minVal) return minVal;
    if (value > maxVal) return maxVal;
    return value;
}

// --- Hysteresis Threshold ---

void sigCondHysteresisInit(SigCondHysteresis* h, int threshHigh, int threshLow) {
    h->threshHigh = threshHigh;
    h->threshLow = threshLow;
    h->state = 0;
}

uint8_t sigCondHysteresisApply(SigCondHysteresis* h, int value) {
    if (h->state == 0 && value >= h->threshHigh) {
        h->state = 1;
    } else if (h->state == 1 && value <= h->threshLow) {
        h->state = 0;
    }
    return h->state;
}

// --- Debounce Counter ---

void sigCondDebounceInit(SigCondDebounce* d, uint8_t maxCount) {
    d->counter = 0;
    d->maxCount = maxCount;
    d->confirmedState = 0;
}

uint8_t sigCondDebounceApply(SigCondDebounce* d, uint8_t rawState) {
    if (rawState) {
        if (d->counter < d->maxCount) d->counter++;
    } else {
        if (d->counter > 0) d->counter--;
    }

    if (d->counter >= d->maxCount) {
        d->confirmedState = 1;
    } else if (d->counter == 0) {
        d->confirmedState = 0;
    }
    return d->confirmedState;
}
