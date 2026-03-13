#include "srv_dist_monitor.h"
#include "dd_hcsr04.h"
#include "dd_led.h"
#include "lib_sig_cond.h"

#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// --- Internal state ---

static HcSr04* sensor = nullptr;
static Led* redLed = nullptr;
static Led* greenLed = nullptr;

static SigCondBuffer medianBuf;
static SigCondBuffer avgBuf;
static SigCondHysteresis hysteresis;
static SigCondDebounce debounce;

static const int WEIGHTS[] = {50, 25, 15, 10};
static int satMin, satMax;

// --- Protected shared data ---

static SemaphoreHandle_t dataMutex = nullptr;

static int sharedRaw = 0;
static int sharedSaturated = 0;
static int sharedMedian = 0;
static int sharedFiltered = 0;
static uint8_t sharedAlertRaw = 0;
static uint8_t sharedAlertDebounced = 0;
static uint8_t sharedSensorOk = 0;

// --- Init ---

void srvDistMonitorInit(uint8_t trigPin, uint8_t echoPin,
                        uint8_t redLedPin, uint8_t greenLedPin,
                        int satMinVal, int satMaxVal,
                        int threshHigh, int threshLow,
                        uint8_t debounceMax) {
    sensor = new HcSr04(trigPin, echoPin);
    redLed = new Led(redLedPin);
    greenLed = new Led(greenLedPin);

    satMin = satMinVal;
    satMax = satMaxVal;

    sigCondBufferInit(&medianBuf, 5);
    sigCondBufferInit(&avgBuf, 4);
    sigCondHysteresisInit(&hysteresis, threshHigh, threshLow);
    sigCondDebounceInit(&debounce, debounceMax);

    dataMutex = xSemaphoreCreateMutex();
}

// --- Update (called from acquisition task) ---

void srvDistMonitorUpdate() {
    bool ok = sensor->read();

    if (!ok) {
        xSemaphoreTake(dataMutex, portMAX_DELAY);
        sharedSensorOk = 0;
        xSemaphoreGive(dataMutex);
        return;
    }

    int raw = sensor->getDistanceMm();

    // Conditioning chain
    int saturated = sigCondSaturate(raw, satMin, satMax);
    int median = sigCondMedianFilter(&medianBuf, saturated);

    sigCondBufferInsert(&avgBuf, median);
    int filtered = sigCondWeightedAvg(&avgBuf, WEIGHTS);

    // Threshold alerting chain (alert when too close)
    uint8_t alertRaw = sigCondHysteresisApply(&hysteresis, filtered);
    uint8_t alertDebounced = sigCondDebounceApply(&debounce, alertRaw);

    // Drive alert LEDs: red = too close (alert), green = safe distance
    if (alertDebounced) {
        redLed->on();
        greenLed->off();
    } else {
        redLed->off();
        greenLed->on();
    }

    // Store under mutex
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    sharedRaw = raw;
    sharedSaturated = saturated;
    sharedMedian = median;
    sharedFiltered = filtered;
    sharedAlertRaw = alertRaw;
    sharedAlertDebounced = alertDebounced;
    sharedSensorOk = 1;
    xSemaphoreGive(dataMutex);
}

// --- Thread-safe getters ---

int srvDistGetRaw() {
    int v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedRaw;
    xSemaphoreGive(dataMutex);
    return v;
}

int srvDistGetSaturated() {
    int v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedSaturated;
    xSemaphoreGive(dataMutex);
    return v;
}

int srvDistGetMedian() {
    int v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedMedian;
    xSemaphoreGive(dataMutex);
    return v;
}

int srvDistGetFiltered() {
    int v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedFiltered;
    xSemaphoreGive(dataMutex);
    return v;
}

uint8_t srvDistGetAlertRaw() {
    uint8_t v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedAlertRaw;
    xSemaphoreGive(dataMutex);
    return v;
}

uint8_t srvDistGetAlertDebounced() {
    uint8_t v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedAlertDebounced;
    xSemaphoreGive(dataMutex);
    return v;
}

uint8_t srvDistGetSensorOk() {
    uint8_t v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedSensorOk;
    xSemaphoreGive(dataMutex);
    return v;
}
