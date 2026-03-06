#include "task_2_btn_stats.h"
#include "task_1_btn_dur_det.h"
#include "dd_led.h"
#include <Arduino_FreeRTOS.h>
#include <semphr.h>

#define TASK_2_BLINK_SHORT     5
#define TASK_2_BLINK_LONG      10
#define TASK_2_BLINK_DELAY_MS  100

static Led yellow_led(35);

// Statistics protected by mutex
static int total_count = 0;
static int short_count = 0;
static int long_count = 0;
static unsigned long total_short_duration = 0;
static unsigned long total_long_duration = 0;
static SemaphoreHandle_t stats_mutex = NULL;

static int last_blink_long = 0;

// Read and clear all pending data from task 1, update stats immediately
static void process_pending(void) {
    xSemaphoreTake(task_1_data_mutex, portMAX_DELAY);
    int ps = task_1_pending_short_count;
    int pl = task_1_pending_long_count;
    unsigned long psd = task_1_pending_short_dur;
    unsigned long pld = task_1_pending_long_dur;
    int last_long = task_1_last_is_long;
    task_1_pending_short_count = 0;
    task_1_pending_long_count = 0;
    task_1_pending_short_dur = 0;
    task_1_pending_long_dur = 0;
    xSemaphoreGive(task_1_data_mutex);

    if (ps + pl > 0) {
        xSemaphoreTake(stats_mutex, portMAX_DELAY);
        total_count += ps + pl;
        short_count += ps;
        long_count += pl;
        total_short_duration += psd;
        total_long_duration += pld;
        xSemaphoreGive(stats_mutex);
        last_blink_long = last_long;
    }
}

int task_2_get_total_count(void) {
    int val;
    xSemaphoreTake(stats_mutex, portMAX_DELAY);
    val = total_count;
    xSemaphoreGive(stats_mutex);
    return val;
}

int task_2_get_short_count(void) {
    int val;
    xSemaphoreTake(stats_mutex, portMAX_DELAY);
    val = short_count;
    xSemaphoreGive(stats_mutex);
    return val;
}

int task_2_get_long_count(void) {
    int val;
    xSemaphoreTake(stats_mutex, portMAX_DELAY);
    val = long_count;
    xSemaphoreGive(stats_mutex);
    return val;
}

unsigned long task_2_get_total_duration(void) {
    unsigned long val;
    xSemaphoreTake(stats_mutex, portMAX_DELAY);
    val = total_short_duration + total_long_duration;
    xSemaphoreGive(stats_mutex);
    return val;
}

void task_2_reset_stats(void) {
    xSemaphoreTake(stats_mutex, portMAX_DELAY);
    total_count = 0;
    short_count = 0;
    long_count = 0;
    total_short_duration = 0;
    total_long_duration = 0;
    xSemaphoreGive(stats_mutex);
}

void task_2_btn_stats_setup(void) {
    stats_mutex = xSemaphoreCreateMutex();
    total_count = 0;
    short_count = 0;
    long_count = 0;
    total_short_duration = 0;
    total_long_duration = 0;
    last_blink_long = 0;
    yellow_led.off();
}

void task_2_btn_stats_loop(void) {
    // Block until task 1 signals at least one press pending
    if (xSemaphoreTake(task_1_press_semphr, portMAX_DELAY) == pdTRUE) {
        process_pending();

        // Blink yellow LED: 5 for short, 10 for long
        // Reset counter on new press (like bare-metal blink_cnt reset)
        int blink_cnt = (last_blink_long ? TASK_2_BLINK_LONG : TASK_2_BLINK_SHORT) * 2;
        int i = 0;
        while (i < blink_cnt) {
            yellow_led.toggle();
            vTaskDelay(pdMS_TO_TICKS(TASK_2_BLINK_DELAY_MS));
            i++;
            // If new press arrived, restart blink for the new press
            if (xSemaphoreTake(task_1_press_semphr, 0) == pdTRUE) {
                process_pending();
                blink_cnt = (last_blink_long ? TASK_2_BLINK_LONG : TASK_2_BLINK_SHORT) * 2;
                i = 0;
                yellow_led.off();
            }
        }
        yellow_led.off();
    }
}
