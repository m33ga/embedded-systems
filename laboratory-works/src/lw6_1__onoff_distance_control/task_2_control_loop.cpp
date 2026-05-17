#include "task_2_control_loop.h"
#include "srv_thermistor_monitor.h"
#include "srv_relay_actuator.h"
#include "srv_onoff_controller.h"
#include "dd_led.h"

// Thermistor config
#define THERMISTOR_PIN   A0

// Temperature monitor alert LEDs (overheat)
#define TEMP_RED_LED     26
#define TEMP_GREEN_LED   28

// Temperature saturation range (tenths °C)
#define TEMP_SAT_MIN     100   // 10.0°C
#define TEMP_SAT_MAX     500   // 50.0°C

// Overheat alert thresholds (tenths °C)
#define TEMP_THRESH_HIGH 450   // 45.0°C
#define TEMP_THRESH_LOW  400   // 40.0°C
#define TEMP_DEBOUNCE    3

// Relay config
#define RELAY_PIN        24
#define RELAY_NC         false
#define RELAY_DEBOUNCE   3

// ON-OFF controller config
#define HYST_BAND        5     // ±0.5°C hysteresis around setpoint
#define ALERT_THRESHOLD  30    // deviation alert when |error| > 3.0°C

// Warning LED (large deviation)
#define WARN_LED_PIN     30

static Led* warnLed = nullptr;

void task_2_control_loop_setup(void) {
    srvThermistorMonitorInit(THERMISTOR_PIN,
                             TEMP_RED_LED, TEMP_GREEN_LED,
                             TEMP_SAT_MIN, TEMP_SAT_MAX,
                             TEMP_THRESH_HIGH, TEMP_THRESH_LOW,
                             TEMP_DEBOUNCE);

    srvRelayActuatorInit(RELAY_PIN, RELAY_NC, RELAY_DEBOUNCE);

    srvOnOffControllerInit(HYST_BAND, ALERT_THRESHOLD);

    warnLed = new Led(WARN_LED_PIN);
}

void task_2_control_loop_loop(void) {
    // 1. Acquire and condition temperature
    srvThermistorMonitorUpdate();

    // 2. Get filtered temperature
    int temp = srvThermistorGetFiltered();

    // 3. Run ON-OFF controller
    srvOnOffControllerUpdate(temp);

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
