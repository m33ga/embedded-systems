#include "task_2_lcd_report.h"
#include "srv_temp_monitor.h"
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
    int flt = srvTempGetFiltered();
    int hum = srvTempGetHumidity();
    uint8_t alertDeb = srvTempGetAlertDebounced();
    uint8_t sensorOk = srvTempGetSensorOk();

    srvLCDClear();

    // Line 1: temperature + humidity (16 chars max)
    // e.g. "T:26.5C H:36.4%"
    srvLCDCursor(0, 0);
    if (sensorOk) {
        printTenths("T:", flt);
        printTenths("C H:", hum);
        printf("%%");
    } else {
        printf("Sensor ERROR!   ");
    }

    // Line 2: alert status
    // e.g. "Alert: OFF      " or "Alert: ON       "
    srvLCDCursor(0, 1);
    printf("Alert: %s", alertDeb ? "ON " : "OFF");
}
