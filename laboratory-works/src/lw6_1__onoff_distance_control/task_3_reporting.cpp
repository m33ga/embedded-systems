#include "task_3_reporting.h"
#include "srv_thermistor_monitor.h"
#include "srv_relay_actuator.h"
#include "srv_onoff_controller.h"
#include "srv_lcd_stdio.h"
#include <stdio.h>

void task_3_reporting_setup(void) {
    srvLCDClear();
}

void task_3_reporting_loop(void) {
    int sp       = srvOnOffControllerGetSetpoint();
    int temp     = srvThermistorGetFiltered();
    uint8_t rly  = srvRelayActuatorGetState();
    uint8_t warn = srvOnOffControllerGetDeviationAlert();
    int error    = srvOnOffControllerGetError();

    srvLCDSetup();
    srvLCDClear();

    // Line 1: setpoint and temperature (tenths → decimal)
    srvLCDCursor(0, 0);
    printf("SP:%d.%d T:%d.%d", sp / 10, sp % 10, temp / 10, temp % 10);

    // Line 2: relay state + error + warning
    srvLCDCursor(0, 1);
    printf("Rly:%s E:%-3d %s", rly ? "ON " : "OFF",
           error, warn ? "W" : " ");
}
