#ifndef SRV_PID_CONTROLLER_H
#define SRV_PID_CONTROLLER_H

#include <stdint.h>

void srvPidControllerInit(float kp, float ki, float kd,
                          int outMin, int outMax, int dtMs);

void srvPidControllerSetSetpoint(int sp);
void srvPidControllerSetGains(float kp, float ki, float kd);

// Run one PID cycle; measured is in tenths °C
void srvPidControllerUpdate(int measured);

int srvPidControllerGetSetpoint();
int srvPidControllerGetOutput();
int srvPidControllerGetError();
float srvPidControllerGetKp();
float srvPidControllerGetKi();
float srvPidControllerGetKd();

#endif
