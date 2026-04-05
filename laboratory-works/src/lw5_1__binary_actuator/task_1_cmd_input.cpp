#include "task_1_cmd_input.h"
#include "srv_relay_actuator.h"
#include <stdio.h>
#include <string.h>

void task_1_cmd_input_setup(void) {}

void task_1_cmd_input_loop(void) {
    char buf[8];
    int i = 0;
    int c;

    while (i < 7) {
        c = getchar();
        if (c == '\n' || c == EOF) break;
        buf[i++] = (char)c;
    }
    buf[i] = '\0';

    if (strcmp(buf, "1") == 0 || strcmp(buf, "ON") == 0) {
        srvRelayActuatorSetCommand(1);
    } else if (strcmp(buf, "0") == 0 || strcmp(buf, "OFF") == 0) {
        srvRelayActuatorSetCommand(0);
    }
}
