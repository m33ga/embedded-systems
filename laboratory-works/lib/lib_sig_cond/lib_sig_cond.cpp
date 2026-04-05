#include "lib_sig_cond.h"
#include <string.h>

// --- Saturation ---

int sigCondSaturate(int value, int minVal, int maxVal) {
    if (value < minVal) return minVal;
    if (value > maxVal) return maxVal;
    return value;
}

// --- Circular Buffer ---

void sigCondBufferInit(SigCondBuffer* buf, uint8_t size) {
    if (size > SIG_COND_BUF_MAX) size = SIG_COND_BUF_MAX;
    buf->size = size;
    buf->head = 0;
    buf->count = 0;
    memset(buf->data, 0, sizeof(buf->data));
}

void sigCondBufferInsert(SigCondBuffer* buf, int value) {
    buf->data[buf->head] = value;
    buf->head = (buf->head + 1) % buf->size;
    if (buf->count < buf->size) buf->count++;
}

void sigCondBufferCopy(const SigCondBuffer* buf, int* dest) {
    for (uint8_t i = 0; i < buf->count; i++) {
        dest[i] = buf->data[i];
    }
}

// --- Sorting ---

void sigCondBubbleSort(int* arr, uint8_t len) {
    for (uint8_t i = 0; i < len - 1; i++) {
        for (uint8_t j = 0; j < len - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

// --- Median ---

int sigCondMedian(const int* sortedArr, uint8_t len) {
    if (len == 0) return 0;
    if (len % 2 == 1) return sortedArr[len / 2];
    return (sortedArr[len / 2 - 1] + sortedArr[len / 2]) / 2;
}

int sigCondMedianFilter(SigCondBuffer* buf, int newValue) {
    sigCondBufferInsert(buf, newValue);
    int tmp[SIG_COND_BUF_MAX];
    sigCondBufferCopy(buf, tmp);
    sigCondBubbleSort(tmp, buf->count);
    return sigCondMedian(tmp, buf->count);
}

// --- Weighted Average ---

int sigCondWeightedAvg(const SigCondBuffer* buf, const int* weights) {
    if (buf->count == 0) return 0;
    long sumUp = 0;
    long sumDn = 0;
    // weights[0] = newest, weights[count-1] = oldest
    // buffer stores newest at (head-1), oldest at (head - count)
    for (uint8_t i = 0; i < buf->count; i++) {
        // index from newest to oldest
        uint8_t idx = (buf->head + buf->size - 1 - i) % buf->size;
        sumUp += (long)buf->data[idx] * weights[i];
        sumDn += weights[i];
    }
    if (sumDn == 0) return 0;
    return (int)(sumUp / sumDn);
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

// --- Ramp (smooth transition) ---

void sigCondRampInit(SigCondRamp* r, int startValue, int step) {
    r->current = startValue;
    r->step = step;
}

int sigCondRampApply(SigCondRamp* r, int target) {
    if (r->current < target) {
        r->current += r->step;
        if (r->current > target) r->current = target;
    } else if (r->current > target) {
        r->current -= r->step;
        if (r->current < target) r->current = target;
    }
    return r->current;
}
