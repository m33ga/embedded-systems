#include "task_2_control_loop.h"
#include "srv_dist_monitor.h"
#include "srv_relay_actuator.h"
#include "srv_onoff_controller.h"
#include "dd_led.h"

// HC-SR04 pins
#define TRIG_PIN         22
#define ECHO_PIN         23

// Distance monitor alert LEDs (sensor status)
#define DIST_RED_LED     26
#define DIST_GREEN_LED   28

// Distance saturation range (mm)
#define DIST_SAT_MIN     20
#define DIST_SAT_MAX     4000

// Distance monitor hysteresis thresholds (not used for control, just sensor alert)
#define DIST_THRESH_HIGH 3500
#define DIST_THRESH_LOW  3000
#define DIST_DEBOUNCE    3

// Relay config
#define RELAY_PIN        24
#define RELAY_NC         false
#define RELAY_DEBOUNCE   3

// ON-OFF controller config
#define HYST_BAND        20    // ±20mm hysteresis around setpoint
#define ALERT_THRESHOLD  50    // deviation alert when |error| > 50mm

// Warning LED (large deviation)
#define WARN_LED_PIN     30

static Led* warnLed = nullptr;

void task_2_control_loop_setup(void) {
    srvDistMonitorInit(TRIG_PIN, ECHO_PIN,
                       DIST_RED_LED, DIST_GREEN_LED,
                       DIST_SAT_MIN, DIST_SAT_MAX,
                       DIST_THRESH_HIGH, DIST_THRESH_LOW,
                       DIST_DEBOUNCE);

    srvRelayActuatorInit(RELAY_PIN, RELAY_NC, RELAY_DEBOUNCE);

    srvOnOffControllerInit(HYST_BAND, ALERT_THRESHOLD);

    warnLed = new Led(WARN_LED_PIN);
}

void task_2_control_loop_loop(void) {
    // 1. Acquire and condition distance
    srvDistMonitorUpdate();

    // 2. Get filtered distance
    int dist = srvDistGetFiltered();

    // 3. Run ON-OFF controller
    srvOnOffControllerUpdate(dist);

    // 4. Drive relay from controller output
    uint8_t output = srvOnOffControllerGetOutput();
    srvRelayActuatorSetCommand(output);
    srvRelayActuatorUpdate();

    // 5. Drive warning LED on large deviation
    uint8_t devAlert = srvOnOffControllerGetDeviationAlert();
    if (devAlert) {
        warnLed->on();
    } else {
        warnLed->off();
    }
}
