#include "task_1_sensor_acq.h"
#include "srv_dist_monitor.h"
#include "srv_light_monitor.h"
#include <Arduino.h>

// --- HC-SR04 Distance Sensor ---
#define DIST_TRIG_PIN       39
#define DIST_ECHO_PIN       41
#define RED_LED_PIN         31
#define GREEN_LED_PIN       33

// Distance thresholds in mm: alert when closer than 200mm (20cm)
#define DIST_THRESH_HIGH    250   // 25cm - above this = safe (alert off)
#define DIST_THRESH_LOW     150   // 15cm - below this = too close (alert on)

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
    // Initialize distance monitor (with LED control)
    srvDistMonitorInit(DIST_TRIG_PIN, DIST_ECHO_PIN,
                       RED_LED_PIN, GREEN_LED_PIN,
                       DIST_SAT_MIN, DIST_SAT_MAX,
                       DIST_THRESH_HIGH, DIST_THRESH_LOW,
                       DIST_DEBOUNCE_MAX);

    // Initialize light monitor (no LED)
    srvLightMonitorInit(LIGHT_SENSOR_PIN,
                        LIGHT_SAT_MIN, LIGHT_SAT_MAX,
                        LIGHT_HYST_PERCENT,
                        LIGHT_DEBOUNCE_MAX);
}

void task_1_sensor_acq_loop(void) {
    srvDistMonitorUpdate();
    srvLightMonitorUpdate();
}
