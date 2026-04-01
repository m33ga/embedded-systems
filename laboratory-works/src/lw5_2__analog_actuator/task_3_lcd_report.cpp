#include "task_3_lcd_report.h"
#include "srv_relay_actuator.h"
#include "srv_motor_actuator.h"
#include "srv_lcd_stdio.h"
#include <stdio.h>

void task_3_lcd_report_setup(void) {
    srvLCDClear();
}

void task_3_lcd_report_loop(void) {
    uint8_t relayState = srvRelayActuatorGetState();
    int motorRamped = srvMotorActuatorGetRamped();
    uint8_t motorSatAlert = srvMotorActuatorGetAlertSaturation();

    srvLCDClear();

    // Line 1: Relay status (16 chars max)
    srvLCDCursor(0, 0);
    printf("Relay: %s", relayState ? "ON " : "OFF");

    // Line 2: Motor speed percentage + direction + alert
    // Format: "M:+075%% S" or "M:-050%%  "
    srvLCDCursor(0, 1);
    char sign = '+';
    int absSpeed = motorRamped;
    if (motorRamped < 0) { sign = '-'; absSpeed = -motorRamped; }
    printf("M:%c%03d%% %s", sign, absSpeed, motorSatAlert ? "SAT" : "   ");
}
