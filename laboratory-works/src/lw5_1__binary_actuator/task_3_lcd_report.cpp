#include "task_3_lcd_report.h"
#include "srv_relay_actuator.h"
#include "srv_lcd_stdio.h"
#include <stdio.h>

void task_3_lcd_report_setup(void) {
    srvLCDClear();
}

void task_3_lcd_report_loop(void) {
    uint8_t state = srvRelayActuatorGetState();
    uint8_t cmdRaw = srvRelayActuatorGetCommandRaw();
    uint8_t cmdDeb = srvRelayActuatorGetCommandDebounced();

    srvLCDClear();

    srvLCDCursor(0, 0);
    printf("Relay: %s", state ? "ON " : "OFF");

    srvLCDCursor(0, 1);
    printf("Cmd:%d Deb:%d", cmdRaw, cmdDeb);
}
