#ifndef SRV_THERMISTOR_MONITOR_H
#define SRV_THERMISTOR_MONITOR_H

#include <stdint.h>

// All temperature values are in tenths of degree (e.g. 250 = 25.0C)

void srvThermistorMonitorInit(uint8_t analogPin,
                              uint8_t redLedPin, uint8_t greenLedPin,
                              int satMin, int satMax,
                              int threshHigh, int threshLow,
                              uint8_t debounceMax);

void srvThermistorMonitorUpdate();

int srvThermistorGetRaw();
int srvThermistorGetSaturated();
int srvThermistorGetMedian();
int srvThermistorGetFiltered();
uint8_t srvThermistorGetAlertRaw();
uint8_t srvThermistorGetAlertDebounced();

#endif
