#include "task_1_btn_dur_det.h"
#include "dd_button.h"
#include "dd_led.h"
#include <Arduino.h>

#define TASK_1_DEBOUNCE_MAX   2
#define TASK_1_DEBOUNCE_INC   1
#define TASK_1_DEBOUNCE_MIN   0
#define TASK_1_DEBOUNCE_DEC   1
#define TASK_1_LONG_PRESS_MS  500

static Button btn(22);
static Led green_led(33);
static Led red_led(31);

static int debounce_cnt = 0;
static int btn_confirmed_state = 0;
static unsigned long press_start_ms = 0;

// Sync primitives
SemaphoreHandle_t task_1_press_semphr = NULL;
SemaphoreHandle_t task_1_data_mutex = NULL;

// Shared pending data
int task_1_pending_short_count = 0;
int task_1_pending_long_count = 0;
unsigned long task_1_pending_short_dur = 0;
unsigned long task_1_pending_long_dur = 0;
int task_1_last_is_long = 0;

void task_1_btn_dur_det_setup(void) {
    debounce_cnt = 0;
    btn_confirmed_state = 0;
    press_start_ms = 0;

    task_1_press_semphr = xSemaphoreCreateBinary();
    task_1_data_mutex = xSemaphoreCreateMutex();

    task_1_pending_short_count = 0;
    task_1_pending_long_count = 0;
    task_1_pending_short_dur = 0;
    task_1_pending_long_dur = 0;
    task_1_last_is_long = 0;

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
    }
    else if (debounce_cnt <= TASK_1_DEBOUNCE_MIN && btn_confirmed_state == 1) {
        btn_confirmed_state = 0;
        unsigned long duration = millis() - press_start_ms;
        int is_long = (duration >= TASK_1_LONG_PRESS_MS) ? 1 : 0;

        if (is_long) {
            red_led.on();
            green_led.off();
        } else {
            green_led.on();
            red_led.off();
        }

        // Accumulate pending data under mutex
        xSemaphoreTake(task_1_data_mutex, portMAX_DELAY);
        if (is_long) {
            task_1_pending_long_count++;
            task_1_pending_long_dur += duration;
        } else {
            task_1_pending_short_count++;
            task_1_pending_short_dur += duration;
        }
        task_1_last_is_long = is_long;
        xSemaphoreGive(task_1_data_mutex);

        // Signal task 2 that new data is pending
        xSemaphoreGive(task_1_press_semphr);
    }
}
