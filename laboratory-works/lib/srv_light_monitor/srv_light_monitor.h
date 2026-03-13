#ifndef SRV_LIGHT_MONITOR_H
#define SRV_LIGHT_MONITOR_H

#include <stdint.h>

// Initialize light monitor
// hysteresisPercent: percentage deviation from baseline to trigger alert (e.g., 20 = ±20%)
void srvLightMonitorInit(uint8_t sensorPin,
                         int satMin, int satMax,
                         uint8_t hysteresisPercent,
                         uint8_t debounceMax);

// Run one acquisition + conditioning cycle
void srvLightMonitorUpdate();

// Thread-safe getters
int srvLightGetRaw();           // Raw ADC value (0-1023)
int srvLightGetPercent();       // As percentage (0-100)
int srvLightGetSaturated();
int srvLightGetMedian();
int srvLightGetFiltered();
int srvLightGetBaseline();      // First-read calibration value
uint8_t srvLightGetAlertRaw();
uint8_t srvLightGetAlertDebounced();
uint8_t srvLightIsCalibrated(); // True after first successful read

#endif
