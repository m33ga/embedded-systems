#include "task_1_sensor_acq.h"
#include "srv_temp_monitor.h"

#define SENSOR_PIN      39
#define RED_LED_PIN     31
#define GREEN_LED_PIN   33

// Temperature thresholds in tenths of degree: 25.0°C ± 1.0°C
#define THRESH_HIGH     260   // 26.0°C
#define THRESH_LOW      240   // 24.0°C

// Saturation limits in tenths of degree
#define SAT_MIN         -100  // -10.0°C
#define SAT_MAX         600   //  60.0°C

#define DEBOUNCE_MAX    3

void task_1_sensor_acq_setup(void) {
    srvTempMonitorInit(SENSOR_PIN,
                       RED_LED_PIN, GREEN_LED_PIN,
                       SAT_MIN, SAT_MAX,
                       THRESH_HIGH, THRESH_LOW,
                       DEBOUNCE_MAX);
}

void task_1_sensor_acq_loop(void) {
    srvTempMonitorUpdate();
}
