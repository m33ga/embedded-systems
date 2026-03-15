#include "task_2_lcd_report.h"
#include "srv_temp_monitor.h"
#include "srv_light_monitor.h"
#include "srv_lcd_stdio.h"
#include <stdio.h>
#include <stdlib.h>

void task_2_lcd_report_setup(void) {
    srvLCDClear();
}

void task_2_lcd_report_loop(void) {
    // Get temperature data (in °C)
    int tempFlt = srvTempGetFiltered();
    uint8_t tempAlert = srvTempGetAlertDebounced();
    uint8_t tempOk = srvTempGetSensorOk();

    // Get light data
    int lightFlt = srvLightGetFiltered();
    uint8_t lightAlert = srvLightGetAlertDebounced();

    srvLCDClear();

    // Line 1: Temperature + Light readings (16 chars max)
    // Format: "T:25°C L:45%", divide temp by 10 to get °C from tenths of degree
    srvLCDCursor(0, 0);
    if (tempOk) {
        printf("T:%.1fC L:%d%%", tempFlt / 10.0, lightFlt);
    } else {
        printf("T:ERR L:%d%%", lightFlt);
    }

    // Line 2: Alert status for both sensors
    // Format: "TA:OFF LA:ON"
    srvLCDCursor(0, 1);
    printf("TA:%s LA:%s",
           tempAlert ? "ON " : "OFF",
           lightAlert ? "ON " : "OFF");
}
