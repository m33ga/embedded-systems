#include "task_1_sensor_acq.h"
#include "srv_temp_monitor.h"
#include "srv_light_monitor.h"
#include <Arduino.h>

// --- DHT11 Temperature Sensor ---
#define DHT_SENSOR_PIN      39
#define RED_LED_PIN         31
#define GREEN_LED_PIN       33

// Temperature thresholds in tenths of degree: 25.0°C ± 1.0°C
#define TEMP_THRESH_HIGH    260   // 26.0°C
#define TEMP_THRESH_LOW     240   // 24.0°C

// Saturation limits in tenths of degree
#define TEMP_SAT_MIN        -100  // -10.0°C
#define TEMP_SAT_MAX        600   //  60.0°C

#define TEMP_DEBOUNCE_MAX   3

// --- Photoresistor Light Sensor ---
#define LIGHT_SENSOR_PIN    A0

// Saturation limits for percentage (0-100)
#define LIGHT_SAT_MIN       0
#define LIGHT_SAT_MAX       100

// Hysteresis: ±5% from baseline
#define LIGHT_HYST_PERCENT  5

#define LIGHT_DEBOUNCE_MAX  3

void task_1_sensor_acq_setup(void) {
    // Initialize temperature monitor (with LED control)
    srvTempMonitorInit(DHT_SENSOR_PIN,
                       RED_LED_PIN, GREEN_LED_PIN,
                       TEMP_SAT_MIN, TEMP_SAT_MAX,
                       TEMP_THRESH_HIGH, TEMP_THRESH_LOW,
                       TEMP_DEBOUNCE_MAX);

    // Initialize light monitor (no LED)
    srvLightMonitorInit(LIGHT_SENSOR_PIN,
                        LIGHT_SAT_MIN, LIGHT_SAT_MAX,
                        LIGHT_HYST_PERCENT,
                        LIGHT_DEBOUNCE_MAX);
}

void task_1_sensor_acq_loop(void) {
    srvTempMonitorUpdate();
    srvLightMonitorUpdate();
}
