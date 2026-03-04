#include "task_1_btn_dur_det.h"
#include "dd_button.h"
#include "dd_led.h"
#include <Arduino.h>

#define TASK_1_DEBOUNCE_MAX   3
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

// Shared press data protected by mutex
static unsigned long last_press_duration = 0;
static int last_press_was_long = 0;
static SemaphoreHandle_t task_1_data_mutex = NULL;

// Binary semaphore for signaling detection to task 2
SemaphoreHandle_t task_1_detected_semphr = NULL;

unsigned long task_1_get_last_duration(void) {
    unsigned long dur;
    xSemaphoreTake(task_1_data_mutex, portMAX_DELAY);
    dur = last_press_duration;
    xSemaphoreGive(task_1_data_mutex);
    return dur;
}

int task_1_is_last_press_long(void) {
    int val;
    xSemaphoreTake(task_1_data_mutex, portMAX_DELAY);
    val = last_press_was_long;
    xSemaphoreGive(task_1_data_mutex);
    return val;
}

void task_1_btn_dur_det_setup(void) {
    debounce_cnt = 0;
    btn_confirmed_state = 0;
    press_start_ms = 0;
    last_press_duration = 0;
    last_press_was_long = 0;

    task_1_detected_semphr = xSemaphoreCreateBinary();
    task_1_data_mutex = xSemaphoreCreateMutex();

    green_led.off();
    red_led.off();
}

void task_1_btn_dur_det_loop(void) {
    // Debounce: saturating counter
    if (btn.isPressed()) {
        if (debounce_cnt < TASK_1_DEBOUNCE_MAX)
            debounce_cnt += TASK_1_DEBOUNCE_INC;
    } else {
        if (debounce_cnt > TASK_1_DEBOUNCE_MIN)
            debounce_cnt -= TASK_1_DEBOUNCE_DEC;
    }

    // Press edge: counter saturated high, previously released
    if (debounce_cnt >= TASK_1_DEBOUNCE_MAX && btn_confirmed_state == 0) {
        btn_confirmed_state = 1;
        press_start_ms = millis();
    }
    // Release edge: counter saturated low, previously pressed
    else if (debounce_cnt <= TASK_1_DEBOUNCE_MIN && btn_confirmed_state == 1) {
        btn_confirmed_state = 0;
        unsigned long duration = millis() - press_start_ms;
        int is_long = (duration >= TASK_1_LONG_PRESS_MS) ? 1 : 0;

        // Store press data under mutex
        xSemaphoreTake(task_1_data_mutex, portMAX_DELAY);
        last_press_duration = duration;
        last_press_was_long = is_long;
        xSemaphoreGive(task_1_data_mutex);

        // Visual feedback via LEDs
        if (is_long) {
            red_led.on();
            green_led.off();
        } else {
            green_led.on();
            red_led.off();
        }

        // Signal task 2 that a press was detected
        xSemaphoreGive(task_1_detected_semphr);
    }
}
