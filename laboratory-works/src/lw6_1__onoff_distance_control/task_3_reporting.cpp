#include "task_3_reporting.h"
#include "srv_dist_monitor.h"
#include "srv_relay_actuator.h"
#include "srv_onoff_controller.h"
#include "srv_lcd_stdio.h"
#include "srv_serial_stdio.h"
#include <stdio.h>

void task_3_reporting_setup(void) {
    srvLCDClear();
}

void task_3_reporting_loop(void) {
    int sp       = srvOnOffControllerGetSetpoint();
    int dist     = srvDistGetFiltered();
    uint8_t rly  = srvRelayActuatorGetState();
    uint8_t warn = srvOnOffControllerGetDeviationAlert();
    int error    = srvOnOffControllerGetError();
    uint8_t sok  = srvDistGetSensorOk();

    // --- LCD output (stdout is routed to LCD) ---
    srvLCDSetup();
    srvLCDClear();

    // Line 1: SP and distance
    srvLCDCursor(0, 0);
    printf("SP:%-4d D:%-4d", sp, dist);

    // Line 2: relay state + warning
    srvLCDCursor(0, 1);
    printf("Rly:%s %s", rly ? "ON " : "OFF",
           warn ? "W" : " ");

    // --- Serial output (switch stdout to serial, print, switch back) ---
    srvSerialSetup();
    if (!sok) {
        printf("[SENSOR ERR] ");
    }
    printf("SP:%d D:%d Rly:%s Err:%d %s\n",
           sp, dist,
           rly ? "ON" : "OFF",
           error,
           warn ? "WARN" : "OK");
}
