#ifndef LIB_SIG_COND_H
#define LIB_SIG_COND_H

#include <stdint.h>

// --- Saturation ---
int sigCondSaturate(int value, int minVal, int maxVal);

// --- Circular Buffer ---
#define SIG_COND_BUF_MAX 8

typedef struct {
    int data[SIG_COND_BUF_MAX];
    uint8_t size;
    uint8_t head;
    uint8_t count;
} SigCondBuffer;

void sigCondBufferInit(SigCondBuffer* buf, uint8_t size);
void sigCondBufferInsert(SigCondBuffer* buf, int value);
void sigCondBufferCopy(const SigCondBuffer* buf, int* dest);

// --- Sorting ---
void sigCondBubbleSort(int* arr, uint8_t len);

// --- Median Filter ---
int sigCondMedian(const int* sortedArr, uint8_t len);
int sigCondMedianFilter(SigCondBuffer* buf, int newValue);

// --- Weighted Average Filter ---
int sigCondWeightedAvg(const SigCondBuffer* buf, const int* weights);

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

// --- Ramp (smooth transition) ---
typedef struct {
    int current;
    int step;
} SigCondRamp;

void sigCondRampInit(SigCondRamp* r, int startValue, int step);
int sigCondRampApply(SigCondRamp* r, int target);

#endif
