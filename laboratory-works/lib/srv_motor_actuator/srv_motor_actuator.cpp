#include "srv_motor_actuator.h"
#include "dd_motor.h"
#include "lib_sig_cond.h"
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

static Motor* motor = nullptr;

static SigCondBuffer medianBuf;
static SigCondBuffer avgBuf;
static SigCondRamp ramp;

static const int WEIGHTS[] = {50, 25, 15, 10};
static int satMin, satMax;

static SemaphoreHandle_t dataMutex = nullptr;
static int sharedCommandRaw = 0;
static int sharedFiltered = 0;
static int sharedRamped = 0;
static int sharedSpeed = 0;
static int sharedDirection = 0;
static uint8_t sharedAlertSat = 0;

void srvMotorActuatorInit(uint8_t pinA, uint8_t pinB,
                          int satMinVal, int satMaxVal,
                          int rampStep) {
    motor = new Motor(pinA, pinB);
    satMin = satMinVal;
    satMax = satMaxVal;
    sigCondBufferInit(&medianBuf, 5);
    sigCondBufferInit(&avgBuf, 4);
    sigCondRampInit(&ramp, 0, rampStep);
    dataMutex = xSemaphoreCreateMutex();
}

void srvMotorActuatorSetCommand(int cmd) {
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    sharedCommandRaw = cmd;
    xSemaphoreGive(dataMutex);
}

void srvMotorActuatorUpdate() {
    int raw;
    xSemaphoreTake(dataMutex, portMAX_DELAY);
    raw = sharedCommandRaw;
    xSemaphoreGive(dataMutex);

    uint8_t alertSat = (raw != sigCondSaturate(raw, satMin, satMax)) ? 1 : 0;
    int saturated = sigCondSaturate(raw, satMin, satMax);

    int median = sigCondMedianFilter(&medianBuf, saturated);

    sigCondBufferInsert(&avgBuf, median);
    int filtered = sigCondWeightedAvg(&avgBuf, WEIGHTS);

    int ramped = sigCondRampApply(&ramp, filtered);

    // Convert percentage (-100..+100) to PWM (-255..+255)
    int pwm = (int)((long)ramped * 255 / 100);
    motor->setSpeed(pwm);

    xSemaphoreTake(dataMutex, portMAX_DELAY);
    sharedFiltered = filtered;
    sharedRamped = ramped;
    sharedSpeed = motor->getSpeed();
    sharedDirection = motor->getDirection();
    sharedAlertSat = alertSat;
    xSemaphoreGive(dataMutex);
}

int srvMotorActuatorGetCommandRaw() {
    int v; xSemaphoreTake(dataMutex, portMAX_DELAY); v = sharedCommandRaw; xSemaphoreGive(dataMutex); return v;
}

int srvMotorActuatorGetFiltered() {
    int v; xSemaphoreTake(dataMutex, portMAX_DELAY); v = sharedFiltered; xSemaphoreGive(dataMutex); return v;
}

int srvMotorActuatorGetRamped() {
    int v; xSemaphoreTake(dataMutex, portMAX_DELAY); v = sharedRamped; xSemaphoreGive(dataMutex); return v;
}

int srvMotorActuatorGetSpeed() {
    int v; xSemaphoreTake(dataMutex, portMAX_DELAY); v = sharedSpeed; xSemaphoreGive(dataMutex); return v;
}

int srvMotorActuatorGetDirection() {
    int v; xSemaphoreTake(dataMutex, portMAX_DELAY); v = sharedDirection; xSemaphoreGive(dataMutex); return v;
}

uint8_t srvMotorActuatorGetAlertSaturation() {
    uint8_t v; xSemaphoreTake(dataMutex, portMAX_DELAY); v = sharedAlertSat; xSemaphoreGive(dataMutex); return v;
}
