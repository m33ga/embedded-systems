#include "task_1_fsm_eval.h"
#include "app_fsm_button_led.h"
#include "dd_button.h"
#include "dd_led.h"

#define BUTTON_PIN 2
#define LED_PIN    13

static Button btn(BUTTON_PIN);
static Led led(LED_PIN);

static int prevBtnState = 0;

void task_1_fsm_eval_setup(void) {
    fsmButtonLedInit();
    prevBtnState = 0;
    led.off();
}

void task_1_fsm_eval_loop(void) {
    int btnPressed = btn.isPressed() ? 1 : 0;

    // Edge detection: trigger FSM only on press edge (0 -> 1)
    int input = 0;
    if (btnPressed && !prevBtnState) {
        input = 1;
    }
    prevBtnState = btnPressed;

    fsmButtonLedEvaluate(input);

    int output = fsmButtonLedGetOutput();
    if (output) {
        led.on();
    } else {
        led.off();
    }
}

int task_1_get_led_output(void) {
    return fsmButtonLedGetOutput();
}

const char* task_1_get_state_name(void) {
    return fsmButtonLedGetStateName();
}
