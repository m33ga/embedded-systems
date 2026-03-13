#ifndef LIB_SIG_COND_H
#define LIB_SIG_COND_H

#include <stdint.h>

// --- Saturation ---
int sigCondSaturate(int value, int minVal, int maxVal);

// --- Hysteresis Threshold ---
typedef struct {
    int threshHigh;
    int threshLow;
    uint8_t state; // 0 = below, 1 = above
} SigCondHysteresis;

void sigCondHysteresisInit(SigCondHysteresis* h, int threshHigh, int threshLow);
uint8_t sigCondHysteresisApply(SigCondHysteresis* h, int value);

// --- Debounce Counter (for binary signal) ---
typedef struct {
    uint8_t counter;
    uint8_t maxCount;
    uint8_t confirmedState;
} SigCondDebounce;

void sigCondDebounceInit(SigCondDebounce* d, uint8_t maxCount);
uint8_t sigCondDebounceApply(SigCondDebounce* d, uint8_t rawState);

#endif
