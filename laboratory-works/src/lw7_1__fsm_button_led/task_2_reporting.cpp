#include "task_2_reporting.h"
#include "task_1_fsm_eval.h"
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
    int ledOut = task_1_get_led_output();

    // Serial output
    fprintf(serialOut, "State: LED_%s | LED: %s\n",
            stateName,
            ledOut ? "ON" : "OFF");

    // LCD output
    srvLCDClear();
    srvLCDCursor(0, 0);
    fprintf(stdout, "State: LED_%s", stateName);
    srvLCDCursor(0, 1);
    fprintf(stdout, "LED: %s", ledOut ? "ON" : "OFF");
}
