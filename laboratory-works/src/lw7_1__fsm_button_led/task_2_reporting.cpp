#include "task_2_reporting.h"
#include "task_1_fsm_eval.h"
#include "srv_lcd_stdio.h"

#include <stdio.h>

void task_2_reporting_setup(void) {
}

void task_2_reporting_loop(void) {
    const char* stateName = task_1_get_state_name();
    int ledOut = task_1_get_led_output();

    srvLCDClear();
    srvLCDCursor(0, 0);
    printf("State: LED_%s", stateName);
    srvLCDCursor(0, 1);
    printf("LED: %s", ledOut ? "ON" : "OFF");
}
