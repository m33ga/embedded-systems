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

static SigCondHysteresis hysteresis;
static SigCondDebounce debounce;

static int satMin, satMax;

// --- Protected shared data ---

static SemaphoreHandle_t dataMutex = nullptr;

static int sharedRaw = 0;
static int sharedSaturated = 0;
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

    sigCondHysteresisInit(&hysteresis, threshHigh, threshLow);
    sigCondDebounceInit(&debounce, debounceMax);

    dataMutex = xSemaphoreCreateMutex();
}

// --- Update (called from acquisition task) ---

void srvTempMonitorUpdate() {
    bool ok = sensor->read();

    if (!ok) {
        // Sensor read failed — skip conditioning, just update status
        xSemaphoreTake(dataMutex, portMAX_DELAY);
        sharedSensorOk = 0;
        xSemaphoreGive(dataMutex);
        return;
    }

    int raw = sensor->getRawTemperature();
    int humidity = sensor->getRawHumidity();

    // Conditioning chain
    int saturated = sigCondSaturate(raw, satMin, satMax);

    // Threshold alerting chain
    uint8_t alertRaw = sigCondHysteresisApply(&hysteresis, saturated);
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
    sharedHumidity = humidity;
    sharedAlertRaw = alertRaw;
    sharedAlertDebounced = alertDebounced;
    sharedSensorOk = 1;
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
