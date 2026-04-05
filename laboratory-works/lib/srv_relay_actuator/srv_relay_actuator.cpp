#include "srv_relay_actuator.h"
#include "dd_relay.h"
#include "lib_sig_cond.h"
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

static Relay* relay = nullptr;
static SigCondDebounce debounce;

static SemaphoreHandle_t dataMutex = nullptr;
static uint8_t sharedCommandRaw = 0;
static uint8_t sharedCommandDebounced = 0;
static uint8_t sharedState = 0;

void srvRelayActuatorInit(uint8_t relayPin, bool normallyClosed, uint8_t debounceMax) {
    relay = new Relay(relayPin, normallyClosed);
    sigCondDebounceInit(&debounce, debounceMax);
    dataMutex = xSemaphoreCreateMutex();
}

void srvRelayActuatorSetCommand(uint8_t cmd) {
    uint8_t saturated = (uint8_t)sigCondSaturate(cmd, 0, 1);
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    sharedCommandRaw = saturated;
    xSemaphoreGive(dataMutex);
}

void srvRelayActuatorUpdate() {
    uint8_t raw;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    raw = sharedCommandRaw;
    xSemaphoreGive(dataMutex);

    uint8_t debounced = sigCondDebounceApply(&debounce, raw);
    relay->setState(debounced);

    xSemaphoreTake(dataMutex, portMAX_DELAY);
    sharedCommandDebounced = debounced;
    sharedState = relay->isOn() ? 1 : 0;
    xSemaphoreGive(dataMutex);
}

uint8_t srvRelayActuatorGetState() {
    uint8_t v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedState;
    xSemaphoreGive(dataMutex);
    return v;
}

uint8_t srvRelayActuatorGetCommandRaw() {
    uint8_t v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedCommandRaw;
    xSemaphoreGive(dataMutex);
    return v;
}

uint8_t srvRelayActuatorGetCommandDebounced() {
    uint8_t v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedCommandDebounced;
    xSemaphoreGive(dataMutex);
    return v;
}
