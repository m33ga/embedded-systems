#include "task_3_reporting.h"
#include "srv_thermistor_monitor.h"
#include "srv_pid_controller.h"
#include "dd_heater.h"
#include "srv_lcd_stdio.h"
#include <stdio.h>

void task_3_reporting_setup(void) {
    srvLCDClear();
}

void task_3_reporting_loop(void) {
    int sp    = srvPidControllerGetSetpoint();
    int temp  = srvThermistorGetFiltered();
    int out   = srvPidControllerGetOutput();
    int error = srvPidControllerGetError();

    srvLCDClear();

    srvLCDCursor(0, 0);
    printf("SP:%d.%d T:%d.%d", sp / 10, sp % 10, temp / 10, temp % 10);

    srvLCDCursor(0, 1);
    printf("Out:%-3d E:%-4d", out, error);
}
