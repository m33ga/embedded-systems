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
    int raw = srvTempGetRaw();
    int flt = srvTempGetFiltered();
    uint8_t alertDeb = srvTempGetAlertDebounced();
    uint8_t sensorOk = srvTempGetSensorOk();

    srvLCDClear();

    // Line 1: Filtered temperature + humidity
    srvLCDCursor(0, 0);
    if (sensorOk) {
        printTenths("T:", flt);
    } else {
        printf("Sensor ERROR!   ");
    }

    // Line 2: Alert status + raw reading
    srvLCDCursor(0, 1);
    printf("A:%s", alertDeb ? "ON " : "OFF");
    printTenths(" R:", raw);
}
