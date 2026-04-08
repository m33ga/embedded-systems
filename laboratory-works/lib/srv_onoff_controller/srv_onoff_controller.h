#ifndef SRV_ONOFF_CONTROLLER_H
#define SRV_ONOFF_CONTROLLER_H

#include <stdint.h>

// All values in mm (distance domain)

void srvOnOffControllerInit(int hystBand, int alertThreshold);

// Thread-safe setpoint setter (mm)
void srvOnOffControllerSetSetpoint(int sp);

// Run one control cycle with the latest measured value (mm)
void srvOnOffControllerUpdate(int measured);

// Thread-safe getters
int srvOnOffControllerGetSetpoint();
uint8_t srvOnOffControllerGetOutput();   // 0 = OFF, 1 = ON
int srvOnOffControllerGetError();
uint8_t srvOnOffControllerGetDeviationAlert();

#endif
