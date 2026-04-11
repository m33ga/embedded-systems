#include "task_1_cmd_input.h"
#include "srv_pid_controller.h"
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

    if (i < 2) return;

    // SP<value> — setpoint in tenths °C (e.g. SP300 = 30.0°C)
    if ((buf[0] == 'S' || buf[0] == 's') &&
        (buf[1] == 'P' || buf[1] == 'p') && i > 2) {
        int val = atoi(&buf[2]);
        if (val >= 150 && val <= 500) {
            srvPidControllerSetSetpoint(val);
        }
        return;
    }

    // P<value> — set Kp (e.g. P10.0)
    if ((buf[0] == 'P' || buf[0] == 'p') && buf[1] >= '0' && buf[1] <= '9') {
        float kp = (float)atof(&buf[1]);
        srvPidControllerSetGains(kp,
                                  srvPidControllerGetKi(),
                                  srvPidControllerGetKd());
        return;
    }

    // I<value> — set Ki (e.g. I0.5)
    if ((buf[0] == 'I' || buf[0] == 'i') && buf[1] >= '0' && buf[1] <= '9') {
        float ki = (float)atof(&buf[1]);
        srvPidControllerSetGains(srvPidControllerGetKp(),
                                  ki,
                                  srvPidControllerGetKd());
        return;
    }

    // D<value> — set Kd (e.g. D2.0)
    if ((buf[0] == 'D' || buf[0] == 'd') && buf[1] >= '0' && buf[1] <= '9') {
        float kd = (float)atof(&buf[1]);
        srvPidControllerSetGains(srvPidControllerGetKp(),
                                  srvPidControllerGetKi(),
                                  kd);
        return;
    }
}
