#include "task_2_reporting.h"
#include "task_1_fsm_traffic.h"
#include "app_fsm_traffic_light.h"
#include "srv_lcd_stdio.h"

#include <Arduino.h>
#include <stdio.h>

static FILE* serialOut = NULL;

static int srvSerialPutCharLocal(char c, FILE* stream) {
    if (c == '\n') Serial.write('\r');
    Serial.write(c);
    return 0;
}

void task_2_reporting_setup(void) {
    static FILE serial_report;
    fdev_setup_stream(&serial_report, srvSerialPutCharLocal, NULL, _FDEV_SETUP_WRITE);
    serialOut = &serial_report;
}

void task_2_reporting_loop(void) {
    const char* stateName = task_1_get_state_name();
    int ewOut = task_1_get_output_ew();
    int nsOut = task_1_get_output_ns();
    const char* ewName = fsmTrafficGetLightName(ewOut);
    const char* nsName = fsmTrafficGetLightName(nsOut);

    // Serial output
    fprintf(serialOut, "[%s] EW:%s NS:%s\n", stateName, ewName, nsName);

    // LCD output
    srvLCDClear();
    srvLCDCursor(0, 0);
    fprintf(stdout, "EW:%s NS:%s", ewName, nsName);
    srvLCDCursor(0, 1);
    fprintf(stdout, "St:%s", stateName);
}
