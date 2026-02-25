#include "task_1_btn_dur_det.h"
#include "dd_button.h"
#include "dd_led.h"
#include <Arduino.h>

#define TASK_1_DEBOUNCE_MAX     3
#define TASK_1_DEBOUNCE_INC     1
#define TASK_1_DEBOUNCE_MIN     0
#define TASK_1_DEBOUNCE_DEC     1
#define TASK_1_LONG_PRESS_MS    500

static Button btn(22);
static Led green_led(33);
static Led red_led(31);

static int debounce_cnt = 0;
static int btn_confirmed_state = 0;
static unsigned long press_start_ms = 0;
static unsigned long last_press_duration = 0;
static int detected_flag = 0;
static int last_press_was_long = 0;

int task_1_get_detected_flag(void) {
    return detected_flag;
}

int task_1_reset_detected_flag(void) {
    return detected_flag = 0;
}

unsigned long task_1_get_last_duration(void) {
    return last_press_duration;
}

int task_1_is_last_press_long(void) {
    return last_press_was_long;
}

void task_1_btn_dur_det_setup(void) {
    debounce_cnt = 0;
    btn_confirmed_state = 0;
    press_start_ms = 0;
    last_press_duration = 0;
    detected_flag = 0;
    last_press_was_long = 0;
    green_led.off();
    red_led.off();
}

void task_1_btn_dur_det_loop(void) {
    if (btn.isPressed()) {
        if (debounce_cnt < TASK_1_DEBOUNCE_MAX)
            debounce_cnt += TASK_1_DEBOUNCE_INC;
    } else {
        if (debounce_cnt > TASK_1_DEBOUNCE_MIN)
            debounce_cnt -= TASK_1_DEBOUNCE_DEC;
    }

    if (debounce_cnt >= TASK_1_DEBOUNCE_MAX && btn_confirmed_state == 0) {
        btn_confirmed_state = 1;
        press_start_ms = millis();
    } else if (debounce_cnt <= TASK_1_DEBOUNCE_MIN && btn_confirmed_state == 1) {
        btn_confirmed_state = 0;
        last_press_duration = millis() - press_start_ms;
        last_press_was_long = (last_press_duration >= TASK_1_LONG_PRESS_MS) ? 1 : 0;
        detected_flag = 1;

        if (last_press_was_long) {
            red_led.on();
            green_led.off();
        } else {
            green_led.on();
            red_led.off();
        }
    }
}
