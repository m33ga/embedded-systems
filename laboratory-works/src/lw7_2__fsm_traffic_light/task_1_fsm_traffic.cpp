#include "task_1_fsm_traffic.h"
#include "app_fsm_traffic_light.h"
#include "dd_button.h"
#include "dd_led.h"

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>

#define NS_REQUEST_PIN  2

#define EW_RED_PIN      22
#define EW_YELLOW_PIN   24
#define EW_GREEN_PIN    26
#define NS_RED_PIN      28
#define NS_YELLOW_PIN   30
#define NS_GREEN_PIN    32

static Button nsRequestBtn(NS_REQUEST_PIN);

static Led ewRed(EW_RED_PIN);
static Led ewYellow(EW_YELLOW_PIN);
static Led ewGreen(EW_GREEN_PIN);
static Led nsRed(NS_RED_PIN);
static Led nsYellow(NS_YELLOW_PIN);
static Led nsGreen(NS_GREEN_PIN);

static Led* ewLeds[3] = {&ewRed, &ewYellow, &ewGreen};
static Led* nsLeds[3] = {&nsRed, &nsYellow, &nsGreen};

static void driveDirection(Led* leds[], int output) {
    leds[0]->off();
    leds[1]->off();
    leds[2]->off();
    leds[output]->on();
}

void task_1_fsm_traffic_setup(void) {
    fsmTrafficInit();
    driveDirection(ewLeds, LIGHT_GREEN);
    driveDirection(nsLeds, LIGHT_RED);
}

void task_1_fsm_traffic_loop(void) {
    // 1. Output based on current state (Moore: output depends only on state)
    driveDirection(ewLeds, fsmTrafficGetOutputEW());
    driveDirection(nsLeds, fsmTrafficGetOutputNS());

    // 2. Wait for state-specific delay
    vTaskDelay(pdMS_TO_TICKS(fsmTrafficGetDelayMs()));

    // 3. Read input (NS crossing request)
    int input = nsRequestBtn.isPressed() ? 1 : 0;

    // 4. Evaluate next state
    fsmTrafficEvaluate(input);
}

const char* task_1_get_state_name(void) {
    return fsmTrafficGetStateName();
}

int task_1_get_output_ew(void) {
    return fsmTrafficGetOutputEW();
}

int task_1_get_output_ns(void) {
    return fsmTrafficGetOutputNS();
}
