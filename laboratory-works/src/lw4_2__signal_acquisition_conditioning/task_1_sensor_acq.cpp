#include "task_1_sensor_acq.h"
#include "srv_temp_monitor.h"
#include "srv_light_monitor.h"
#include <Arduino.h>

// --- DHT11 Temperature Sensor ---
#define TEMP_SENSOR_PIN     39
#define RED_LED_PIN         31
#define GREEN_LED_PIN       33

// Temperature thresholds in °C: alert when above 30°C
#define TEMP_THRESH_HIGH    26   // Above this = alert on
#define TEMP_THRESH_LOW     24   // Below this = alert off

// Saturation limits in mm (2cm to 400cm)
#define DIST_SAT_MIN        20
#define DIST_SAT_MAX        4000

#define DIST_DEBOUNCE_MAX   3

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
    srvTempMonitorInit(TEMP_SENSOR_PIN,
                       RED_LED_PIN, GREEN_LED_PIN,
                       DIST_SAT_MIN, DIST_SAT_MAX,
                       TEMP_THRESH_HIGH, TEMP_THRESH_LOW,
                       DIST_DEBOUNCE_MAX);

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
