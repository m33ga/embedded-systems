#include "task_2_lcd_report.h"
#include "srv_temp_monitor.h"
// #include "srv_lcd_stdio.h"
#include <stdio.h>
#include <stdlib.h>

static void printTenths(const char* prefix, int val) {
    int whole = val / 10;
    int frac = abs(val % 10);
    printf("%s%d.%d", prefix, whole, frac);
}

void task_2_lcd_report_setup(void) {
    // LCD commented out for debug — using serial stdio set up in lw4_1
    // srvLCDInit();
    // srvLCDSetup();
    printf("[Report] task setup done\n");
}

void task_2_lcd_report_loop(void) {
    int raw = srvTempGetRaw();
    int sat = srvTempGetSaturated();
    int med = srvTempGetMedian();
    int flt = srvTempGetFiltered();
    int hum = srvTempGetHumidity();
    uint8_t alertRaw = srvTempGetAlertRaw();
    uint8_t alertDeb = srvTempGetAlertDebounced();
    uint8_t sensorOk = srvTempGetSensorOk();

    printf("--- Sensor Report ---\n");
    printf("Sensor: %s\n", sensorOk ? "OK" : "ERROR");
    printTenths("Raw:  ", raw);
    printf(" C\n");
    printTenths("Sat:  ", sat);
    printf(" C\n");
    printTenths("Med:  ", med);
    printf(" C\n");
    printTenths("Filt: ", flt);
    printf(" C\n");
    printTenths("Hum:  ", hum);
    printf(" %%\n");
    printf("Alert raw: %s | deb: %s\n",
            alertRaw ? "ON" : "OFF", alertDeb ? "ON" : "OFF");
    printf("---------------------\n");
}
