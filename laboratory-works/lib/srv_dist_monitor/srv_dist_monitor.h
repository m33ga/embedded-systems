#ifndef SRV_DIST_MONITOR_H
#define SRV_DIST_MONITOR_H

#include <stdint.h>

// Distance values are in mm for precision

void srvDistMonitorInit(uint8_t trigPin, uint8_t echoPin,
                        uint8_t redLedPin, uint8_t greenLedPin,
                        int satMin, int satMax,
                        int threshHigh, int threshLow,
                        uint8_t debounceMax);

// Run one acquisition + conditioning cycle
void srvDistMonitorUpdate();

// Thread-safe getters (call from reporting task)
int srvDistGetRaw();         // Raw distance in mm
int srvDistGetSaturated();
int srvDistGetMedian();
int srvDistGetFiltered();
uint8_t srvDistGetAlertRaw();
uint8_t srvDistGetAlertDebounced();
uint8_t srvDistGetSensorOk();

#endif
