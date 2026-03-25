#ifndef SRV_RELAY_ACTUATOR_H
#define SRV_RELAY_ACTUATOR_H

#include <stdint.h>

void srvRelayActuatorInit(uint8_t relayPin, bool normallyClosed, uint8_t debounceMax);
void srvRelayActuatorSetCommand(uint8_t cmd);
void srvRelayActuatorUpdate();

uint8_t srvRelayActuatorGetState();
uint8_t srvRelayActuatorGetCommandRaw();
uint8_t srvRelayActuatorGetCommandDebounced();

#endif
