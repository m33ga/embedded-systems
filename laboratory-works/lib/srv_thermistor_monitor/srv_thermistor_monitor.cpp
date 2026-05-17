#include "srv_thermistor_monitor.h"
#include "dd_thermistor.h"
#include "dd_led.h"
#include "lib_sig_cond.h"

#include <Arduino_FreeRTOS.h>
#include <semphr.h>

static Thermistor* sensor = nullptr;
static Led* redLed = nullptr;
static Led* greenLed = nullptr;

static SigCondBuffer medianBuf;
static SigCondBuffer avgBuf;
static SigCondHysteresis hysteresis;
static SigCondDebounce debounce;

static const int WEIGHTS[] = {50, 25, 15, 10};
static int satMin, satMax;

static SemaphoreHandle_t dataMutex = nullptr;

static int sharedRaw = 0;
static int sharedSaturated = 0;
static int sharedMedian = 0;
static int sharedFiltered = 0;
static uint8_t sharedAlertRaw = 0;
static uint8_t sharedAlertDebounced = 0;

void srvThermistorMonitorInit(uint8_t analogPin,
                              uint8_t redLedPin, uint8_t greenLedPin,
                              int satMinVal, int satMaxVal,
                              int threshHigh, int threshLow,
                              uint8_t debounceMax) {
    sensor = new Thermistor(analogPin);
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

void srvThermistorMonitorUpdate() {
    int raw = sensor->readTenths();

    int saturated = sigCondSaturate(raw, satMin, satMax);
    int median = sigCondMedianFilter(&medianBuf, saturated);

    sigCondBufferInsert(&avgBuf, median);
    int filtered = sigCondWeightedAvg(&avgBuf, WEIGHTS);

    uint8_t alertRaw = sigCondHysteresisApply(&hysteresis, filtered);
    uint8_t alertDebounced = sigCondDebounceApply(&debounce, alertRaw);

    // Red = overheat alert, Green = normal temperature
    if (alertDebounced) {
        redLed->on();
        greenLed->off();
    } else {
        redLed->off();
        greenLed->on();
    }

    xSemaphoreTake(dataMutex, portMAX_DELAY);
    sharedRaw = raw;
    sharedSaturated = saturated;
    sharedMedian = median;
    sharedFiltered = filtered;
    sharedAlertRaw = alertRaw;
    sharedAlertDebounced = alertDebounced;
    xSemaphoreGive(dataMutex);
}

int srvThermistorGetRaw() {
    int v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedRaw;
    xSemaphoreGive(dataMutex);
    return v;
}

int srvThermistorGetSaturated() {
    int v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedSaturated;
    xSemaphoreGive(dataMutex);
    return v;
}

int srvThermistorGetMedian() {
    int v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedMedian;
    xSemaphoreGive(dataMutex);
    return v;
}

int srvThermistorGetFiltered() {
    int v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedFiltered;
    xSemaphoreGive(dataMutex);
    return v;
}

uint8_t srvThermistorGetAlertRaw() {
    uint8_t v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedAlertRaw;
    xSemaphoreGive(dataMutex);
    return v;
}

uint8_t srvThermistorGetAlertDebounced() {
    uint8_t v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedAlertDebounced;
    xSemaphoreGive(dataMutex);
    return v;
}
