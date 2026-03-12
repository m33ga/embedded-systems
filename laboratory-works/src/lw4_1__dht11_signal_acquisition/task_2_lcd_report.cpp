#include "task_2_lcd_report.h"
#include "srv_dist_monitor.h"
#include "srv_light_monitor.h"
#include "srv_lcd_stdio.h"
#include <stdio.h>
#include <stdlib.h>

void task_2_lcd_report_setup(void) {
    srvLCDInit();
    srvLCDSetup();
}

void task_2_lcd_report_loop(void) {
    // Get distance data (in mm)
    int distFlt = srvDistGetFiltered();
    uint8_t distAlert = srvDistGetAlertDebounced();
    uint8_t distOk = srvDistGetSensorOk();

    // Get light data
    int lightFlt = srvLightGetFiltered();
    uint8_t lightAlert = srvLightGetAlertDebounced();

    srvLCDClear();

    // Line 1: Distance + Light readings (16 chars max)
    // Format: "D:123mm L:45%"
    srvLCDCursor(0, 0);
    if (distOk) {
        printf("D:%dmm L:%d%%", distFlt, lightFlt);
    } else {
        printf("D:ERR L:%d%%", lightFlt);
    }

    // Line 2: Alert status for both sensors
    // Format: "DA:OFF LA:ON"
    srvLCDCursor(0, 1);
    printf("DA:%s LA:%s",
           distAlert ? "ON " : "OFF",
           lightAlert ? "ON " : "OFF");
}
