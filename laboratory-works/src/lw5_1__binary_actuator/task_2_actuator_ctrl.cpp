#include "task_2_actuator_ctrl.h"
#include "srv_relay_actuator.h"

#define RELAY_PIN        24
#define RELAY_NC         false
#define DEBOUNCE_MAX     3

void task_2_actuator_ctrl_setup(void) {
    srvRelayActuatorInit(RELAY_PIN, RELAY_NC, DEBOUNCE_MAX);
}

void task_2_actuator_ctrl_loop(void) {
    srvRelayActuatorUpdate();
}
