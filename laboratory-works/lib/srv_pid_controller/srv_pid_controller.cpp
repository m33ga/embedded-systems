#include "srv_pid_controller.h"
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

static float pKp = 0, pKi = 0, pKd = 0;
static int pOutMin = 0, pOutMax = 255;
static float pDtSec = 0.1f;

static float integralSum = 0;
static int prevError = 0;

static SemaphoreHandle_t pidMutex = nullptr;
static int sharedSetpoint = 300;
static int sharedOutput = 0;
static int sharedError = 0;

void srvPidControllerInit(float kp, float ki, float kd,
                          int outMin, int outMax, int dtMs) {
    pKp = kp;
    pKi = ki;
    pKd = kd;
    pOutMin = outMin;
    pOutMax = outMax;
    pDtSec = (float)dtMs / 1000.0f;
    integralSum = 0;
    prevError = 0;
    pidMutex = xSemaphoreCreateMutex();
}

void srvPidControllerSetSetpoint(int sp) {
    xSemaphoreTake(pidMutex, portMAX_DELAY);
    sharedSetpoint = sp;
    xSemaphoreGive(pidMutex);
}

void srvPidControllerSetGains(float kp, float ki, float kd) {
    xSemaphoreTake(pidMutex, portMAX_DELAY);
    pKp = kp;
    pKi = ki;
    pKd = kd;
    integralSum = 0;
    xSemaphoreGive(pidMutex);
}

void srvPidControllerUpdate(int measured) {
    int sp;
    float kp, ki, kd;

    xSemaphoreTake(pidMutex, portMAX_DELAY);
    sp = sharedSetpoint;
    kp = pKp;
    ki = pKi;
    kd = pKd;
    xSemaphoreGive(pidMutex);

    int error = sp - measured;

    // Proportional
    float pTerm = kp * (float)error;

    // Integral with anti-windup
    integralSum += (float)error * pDtSec;
    float iTerm = ki * integralSum;

    // Derivative
    float dTerm = kd * (float)(error - prevError) / pDtSec;
    prevError = error;

    // Sum and clamp
    float rawOutput = pTerm + iTerm + dTerm;
    int output;

    if (rawOutput > (float)pOutMax) {
        output = pOutMax;
        // Anti-windup: back-calculate integral
        integralSum -= (float)error * pDtSec;
    } else if (rawOutput < (float)pOutMin) {
        output = pOutMin;
        integralSum -= (float)error * pDtSec;
    } else {
        output = (int)rawOutput;
    }

    xSemaphoreTake(pidMutex, portMAX_DELAY);
    sharedOutput = output;
    sharedError = error;
    xSemaphoreGive(pidMutex);
}

int srvPidControllerGetSetpoint() {
    int v;
    xSemaphoreTake(pidMutex, portMAX_DELAY);
    v = sharedSetpoint;
    xSemaphoreGive(pidMutex);
    return v;
}

int srvPidControllerGetOutput() {
    int v;
    xSemaphoreTake(pidMutex, portMAX_DELAY);
    v = sharedOutput;
    xSemaphoreGive(pidMutex);
    return v;
}

int srvPidControllerGetError() {
    int v;
    xSemaphoreTake(pidMutex, portMAX_DELAY);
    v = sharedError;
    xSemaphoreGive(pidMutex);
    return v;
}

float srvPidControllerGetKp() {
    float v;
    xSemaphoreTake(pidMutex, portMAX_DELAY);
    v = pKp;
    xSemaphoreGive(pidMutex);
    return v;
}

float srvPidControllerGetKi() {
    float v;
    xSemaphoreTake(pidMutex, portMAX_DELAY);
    v = pKi;
    xSemaphoreGive(pidMutex);
    return v;
}

float srvPidControllerGetKd() {
    float v;
    xSemaphoreTake(pidMutex, portMAX_DELAY);
    v = pKd;
    xSemaphoreGive(pidMutex);
    return v;
}
