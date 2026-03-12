#include "srv_light_monitor.h"
#include "dd_photoresistor.h"
#include "lib_sig_cond.h"

#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// --- Internal state ---

static Photoresistor* sensor = nullptr;

static SigCondBuffer medianBuf;
static SigCondBuffer avgBuf;
static SigCondHysteresis hysteresis;
static SigCondDebounce debounce;

static const int WEIGHTS[] = {50, 25, 15, 10};
static int satMin, satMax;
static uint8_t hystPercent;

// Calibration baseline (from first read)
static int baseline = 0;
static uint8_t calibrated = 0;

// --- Protected shared data ---

static SemaphoreHandle_t dataMutex = nullptr;

static int sharedRaw = 0;
static int sharedPercent = 0;
static int sharedSaturated = 0;
static int sharedMedian = 0;
static int sharedFiltered = 0;
static uint8_t sharedAlertRaw = 0;
static uint8_t sharedAlertDebounced = 0;

// --- Init ---

void srvLightMonitorInit(uint8_t sensorPin,
                         int satMinVal, int satMaxVal,
                         uint8_t hysteresisPercent,
                         uint8_t debounceMax) {
    sensor = new Photoresistor(sensorPin);

    satMin = satMinVal;
    satMax = satMaxVal;
    hystPercent = hysteresisPercent;

    sigCondBufferInit(&medianBuf, 5);
    sigCondBufferInit(&avgBuf, 4);
    // Hysteresis thresholds will be set after first read (calibration)
    sigCondHysteresisInit(&hysteresis, 0, 0);
    sigCondDebounceInit(&debounce, debounceMax);

    dataMutex = xSemaphoreCreateMutex();
}

// --- Update (called from acquisition task) ---

void srvLightMonitorUpdate() {
    int raw = sensor->read();
    int percent = sensor->getPercent();

    // On first successful read, establish baseline and set hysteresis thresholds
    if (!calibrated) {
        baseline = percent;
        calibrated = 1;

        // Set thresholds: baseline ± hysteresisPercent
        int threshHigh = baseline + hystPercent;
        int threshLow = baseline - hystPercent;

        // Clamp to valid range
        if (threshHigh > 100) threshHigh = 100;
        if (threshLow < 0) threshLow = 0;

        sigCondHysteresisInit(&hysteresis, threshHigh, threshLow);
    }

    // Conditioning chain (on percentage value for consistency)
    int saturated = sigCondSaturate(percent, satMin, satMax);
    int median = sigCondMedianFilter(&medianBuf, saturated);

    sigCondBufferInsert(&avgBuf, median);
    int filtered = sigCondWeightedAvg(&avgBuf, WEIGHTS);

    // Threshold alerting chain
    uint8_t alertRaw = sigCondHysteresisApply(&hysteresis, filtered);
    uint8_t alertDebounced = sigCondDebounceApply(&debounce, alertRaw);

    // Store under mutex
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    sharedRaw = raw;
    sharedPercent = percent;
    sharedSaturated = saturated;
    sharedMedian = median;
    sharedFiltered = filtered;
    sharedAlertRaw = alertRaw;
    sharedAlertDebounced = alertDebounced;
    xSemaphoreGive(dataMutex);
}

// --- Thread-safe getters ---

int srvLightGetRaw() {
    int v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedRaw;
    xSemaphoreGive(dataMutex);
    return v;
}

int srvLightGetPercent() {
    int v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedPercent;
    xSemaphoreGive(dataMutex);
    return v;
}

int srvLightGetSaturated() {
    int v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedSaturated;
    xSemaphoreGive(dataMutex);
    return v;
}

int srvLightGetMedian() {
    int v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedMedian;
    xSemaphoreGive(dataMutex);
    return v;
}

int srvLightGetFiltered() {
    int v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedFiltered;
    xSemaphoreGive(dataMutex);
    return v;
}

int srvLightGetBaseline() {
    return baseline;
}

uint8_t srvLightGetAlertRaw() {
    uint8_t v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedAlertRaw;
    xSemaphoreGive(dataMutex);
    return v;
}

uint8_t srvLightGetAlertDebounced() {
    uint8_t v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedAlertDebounced;
    xSemaphoreGive(dataMutex);
    return v;
}

uint8_t srvLightIsCalibrated() {
    return calibrated;
}
