#include "srv_onoff_controller.h"
#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <stdlib.h>

static int hystBand = 0;
static int alertThresh = 0;

static SemaphoreHandle_t dataMutex = nullptr;
static int sharedSetpoint = 200;
static uint8_t sharedOutput = 0;
static int sharedError = 0;
static uint8_t sharedDeviationAlert = 0;

void srvOnOffControllerInit(int hystBandVal, int alertThreshold) {
    hystBand = hystBandVal;
    alertThresh = alertThreshold;
    dataMutex = xSemaphoreCreateMutex();
}

void srvOnOffControllerSetSetpoint(int sp) {
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    sharedSetpoint = sp;
    xSemaphoreGive(dataMutex);
}

void srvOnOffControllerUpdate(int measured) {
    int sp;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    sp = sharedSetpoint;
    xSemaphoreGive(dataMutex);

    int error = sp - measured;
    uint8_t output;

    // ON-OFF with hysteresis:
    // measured < (sp - band) => too close => relay ON
    // measured > (sp + band) => far enough => relay OFF
    // otherwise => keep previous state
    if (measured < sp - hystBand) {
        output = 1;
    } else if (measured > sp + hystBand) {
        output = 0;
    } else {
        xSemaphoreTake(dataMutex, portMAX_DELAY);
        output = sharedOutput;
        xSemaphoreGive(dataMutex);
    }

    uint8_t devAlert = (abs(error) > alertThresh) ? 1 : 0;

    xSemaphoreTake(dataMutex, portMAX_DELAY);
    sharedOutput = output;
    sharedError = error;
    sharedDeviationAlert = devAlert;
    xSemaphoreGive(dataMutex);
}

int srvOnOffControllerGetSetpoint() {
    int v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedSetpoint;
    xSemaphoreGive(dataMutex);
    return v;
}

uint8_t srvOnOffControllerGetOutput() {
    uint8_t v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedOutput;
    xSemaphoreGive(dataMutex);
    return v;
}

int srvOnOffControllerGetError() {
    int v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedError;
    xSemaphoreGive(dataMutex);
    return v;
}

uint8_t srvOnOffControllerGetDeviationAlert() {
    uint8_t v;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    v = sharedDeviationAlert;
    xSemaphoreGive(dataMutex);
    return v;
}
