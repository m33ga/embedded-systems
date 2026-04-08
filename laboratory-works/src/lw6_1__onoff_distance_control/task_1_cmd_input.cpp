#include "task_1_cmd_input.h"
#include "srv_onoff_controller.h"
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

    // Setpoint command: SP<value_mm>
    // e.g. SP200 sets setpoint to 200mm
    if ((buf[0] == 'S' || buf[0] == 's') &&
        (buf[1] == 'P' || buf[1] == 'p') && i > 2) {
        int val = atoi(&buf[2]);
        if (val >= 20 && val <= 4000) {
            srvOnOffControllerSetSetpoint(val);
        }
    }
}
