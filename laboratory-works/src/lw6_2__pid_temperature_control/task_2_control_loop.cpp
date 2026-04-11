#include "task_2_control_loop.h"
#include "srv_thermistor_monitor.h"
#include "srv_pid_controller.h"
#include "dd_heater.h"
#include "dd_led.h"

#define THERMISTOR_PIN   A0

#define TEMP_RED_LED     26
#define TEMP_GREEN_LED   28

#define TEMP_SAT_MIN     100   // 10.0°C
#define TEMP_SAT_MAX     500   // 50.0°C

#define TEMP_THRESH_HIGH 450   // 45.0°C
#define TEMP_THRESH_LOW  400   // 40.0°C
#define TEMP_DEBOUNCE    3

#define HEATER_PIN       8

#define PID_KP           10.0f
#define PID_KI           0.5f
#define PID_KD           2.0f
#define PID_OUT_MIN      0
#define PID_OUT_MAX      255
#define PID_DT_MS        100

#define ALERT_THRESHOLD  30    // 3.0°C deviation warning
#define WARN_LED_PIN     30

static Heater* heater = nullptr;
static Led* warnLed = nullptr;

void task_2_control_loop_setup(void) {
    srvThermistorMonitorInit(THERMISTOR_PIN,
                             TEMP_RED_LED, TEMP_GREEN_LED,
                             TEMP_SAT_MIN, TEMP_SAT_MAX,
                             TEMP_THRESH_HIGH, TEMP_THRESH_LOW,
                             TEMP_DEBOUNCE);

    heater = new Heater(HEATER_PIN);

    srvPidControllerInit(PID_KP, PID_KI, PID_KD,
                         PID_OUT_MIN, PID_OUT_MAX, PID_DT_MS);

    warnLed = new Led(WARN_LED_PIN);
}

void task_2_control_loop_loop(void) {
    srvThermistorMonitorUpdate();

    int temp = srvThermistorGetFiltered();

    srvPidControllerUpdate(temp);

    int output = srvPidControllerGetOutput();
    heater->setDuty((uint8_t)output);

    int error = srvPidControllerGetError();
    if (abs(error) > ALERT_THRESHOLD) {
        warnLed->on();
    } else {
        warnLed->off();
    }
}
