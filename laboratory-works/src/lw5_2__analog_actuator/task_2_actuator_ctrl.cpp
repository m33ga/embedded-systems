#include "task_2_actuator_ctrl.h"
#include "srv_relay_actuator.h"
#include "srv_motor_actuator.h"

// Relay config
#define RELAY_PIN        24
#define RELAY_NC         false
#define DEBOUNCE_MAX     3

// Motor config (L9110H H-Bridge)
#define MOTOR_PIN_A      8
#define MOTOR_PIN_B      9
#define MOTOR_SAT_MIN   -100
#define MOTOR_SAT_MAX    100
#define MOTOR_RAMP_STEP  5

void task_2_actuator_ctrl_setup(void) {
    srvRelayActuatorInit(RELAY_PIN, RELAY_NC, DEBOUNCE_MAX);
    srvMotorActuatorInit(MOTOR_PIN_A, MOTOR_PIN_B,
                         MOTOR_SAT_MIN, MOTOR_SAT_MAX,
                         MOTOR_RAMP_STEP);
}

void task_2_actuator_ctrl_loop(void) {
    srvRelayActuatorUpdate();
    srvMotorActuatorUpdate();
}
