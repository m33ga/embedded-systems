#include "task_1_cmd_input.h"
#include "srv_relay_actuator.h"
#include "srv_motor_actuator.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void task_1_cmd_input_setup(void) {}

void task_1_cmd_input_loop(void) {
    char buf[16];
    int i = 0;
    int c;

    while (i < 15) {
        c = getchar();
        if (c == '\n' || c == EOF) break;
        buf[i++] = (char)c;
    }
    buf[i] = '\0';

    // Relay commands: RON, ROFF
    if (strcmp(buf, "RON") == 0) {
        srvRelayActuatorSetCommand(1);
    } else if (strcmp(buf, "ROFF") == 0) {
        srvRelayActuatorSetCommand(0);
    }
    // Motor commands: M<value> where value is -100..+100
    // e.g. M50, M-75, M0
    else if (buf[0] == 'M' || buf[0] == 'm') {
        int val = atoi(&buf[1]);
        srvMotorActuatorSetCommand(val);
    }
}
