#include "task_2_lcd_report.h"
#include "srv_temp_monitor.h"
#include "srv_light_monitor.h"
#include "srv_lcd_stdio.h"
#include <stdio.h>
#include <stdlib.h>

static void printTenths(const char* prefix, int val) {
    int whole = val / 10;
    int frac = abs(val % 10);
    printf("%s%d.%d", prefix, whole, frac);
}

void task_2_lcd_report_setup(void) {
    srvLCDInit();
    srvLCDSetup();
}

void task_2_lcd_report_loop(void) {
    // Get temperature data
    int tempFlt = srvTempGetFiltered();
    uint8_t tempAlert = srvTempGetAlertDebounced();
    uint8_t tempOk = srvTempGetSensorOk();

    // Get light data
    int lightFlt = srvLightGetFiltered();
    uint8_t lightAlert = srvLightGetAlertDebounced();

    srvLCDClear();

    // Line 1: Temperature + Light readings (16 chars max)
    // Format: "T:26.5C L:45%"
    srvLCDCursor(0, 0);
    if (tempOk) {
        printTenths("T:", tempFlt);
        printf("C L:%d%%", lightFlt);
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
