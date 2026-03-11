#include "srv_temp_monitor.h"
#include "dd_dht11.h"
#include "dd_led.h"
#include "lib_sig_cond.h"

#include <Arduino_FreeRTOS.h>
#include <semphr.h>

// --- Internal state ---

static Dht11* sensor = nullptr;
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
static int sharedHumidity = 0;
static uint8_t sharedAlertRaw = 0;
static uint8_t sharedAlertDebounced = 0;
static uint8_t sharedSensorOk = 0;

// --- Init ---

void srvTempMonitorInit(uint8_t sensorPin,
                        uint8_t redLedPin, uint8_t greenLedPin,
                        int satMinVal, int satMaxVal,
                        int threshHigh, int threshLow,
                        uint8_t debounceMax) {
    sensor = new Dht11(sensorPin);
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

void srvTempMonitorUpdate() {
    bool ok = sensor->read();

    int raw = sensor->getRawTemperature();
    int humidity = sensor->getRawHumidity();

    // Conditioning chain
    int saturated = sigCondSaturate(raw, satMin, satMax);
    int median = sigCondMedianFilter(&medianBuf, saturated);

    sigCondBufferInsert(&avgBuf, median);
    int filtered = sigCondWeightedAvg(&avgBuf, WEIGHTS);

    // Threshold alerting chain
    uint8_t alertRaw = sigCondHysteresisApply(&hysteresis, filtered);
    uint8_t alertDebounced = sigCondDebounceApply(&debounce, alertRaw);

    // Drive alert LEDs: red = alert, green = normal
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
    sharedHumidity = humidity;
    sharedAlertRaw = alertRaw;
    sharedAlertDebounced = alertDebounced;
    sharedSensorOk = ok ? 1 : 0;
    xSemaphoreGive(dataMutex);
}

// --- Thread-safe getters ---

int srvTempGetRaw() {
    int v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedRaw;
    xSemaphoreGive(dataMutex);
    return v;
}

int srvTempGetSaturated() {
    int v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedSaturated;
    xSemaphoreGive(dataMutex);
    return v;
}

int srvTempGetMedian() {
    int v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedMedian;
    xSemaphoreGive(dataMutex);
    return v;
}

int srvTempGetFiltered() {
    int v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedFiltered;
    xSemaphoreGive(dataMutex);
    return v;
}

int srvTempGetHumidity() {
    int v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedHumidity;
    xSemaphoreGive(dataMutex);
    return v;
}

uint8_t srvTempGetAlertRaw() {
    uint8_t v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedAlertRaw;
    xSemaphoreGive(dataMutex);
    return v;
}

uint8_t srvTempGetAlertDebounced() {
    uint8_t v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedAlertDebounced;
    xSemaphoreGive(dataMutex);
    return v;
}

uint8_t srvTempGetSensorOk() {
    uint8_t v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedSensorOk;
    xSemaphoreGive(dataMutex);
    return v;
}
