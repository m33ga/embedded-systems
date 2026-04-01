#ifndef SRV_MOTOR_ACTUATOR_H
#define SRV_MOTOR_ACTUATOR_H

#include <stdint.h>

void srvMotorActuatorInit(uint8_t pinA, uint8_t pinB,
                          int satMin, int satMax,
                          int rampStep);
void srvMotorActuatorSetCommand(int cmd);
void srvMotorActuatorUpdate();

int srvMotorActuatorGetCommandRaw();
int srvMotorActuatorGetFiltered();
int srvMotorActuatorGetRamped();
int srvMotorActuatorGetSpeed();
int srvMotorActuatorGetDirection();
uint8_t srvMotorActuatorGetAlertSaturation();

#endif
