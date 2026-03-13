#ifndef SRV_TEMP_MONITOR_H
#define SRV_TEMP_MONITOR_H

#include <stdint.h>

// All temperature values are in tenths of degree (e.g. 250 = 25.0°C)

void srvTempMonitorInit(uint8_t sensorPin,
                        uint8_t redLedPin, uint8_t greenLedPin,
                        int satMin, int satMax,
                        int threshHigh, int threshLow,
                        uint8_t debounceMax);

// Run one acquisition + conditioning cycle
void srvTempMonitorUpdate();

// Thread-safe getters (call from reporting task)
int srvTempGetRaw();
int srvTempGetSaturated();
int srvTempGetMedian();
int srvTempGetFiltered();
int srvTempGetHumidity();
uint8_t srvTempGetAlertRaw();
uint8_t srvTempGetAlertDebounced();
uint8_t srvTempGetSensorOk();

#endif
