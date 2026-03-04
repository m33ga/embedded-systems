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
    yellow_led.off();
}

void task_2_btn_stats_loop(void) {
    // Block until task 1 signals a valid press detection
    if (xSemaphoreTake(task_1_detected_semphr, portMAX_DELAY) == pdTRUE) {
        unsigned long dur = task_1_get_last_duration();
        int is_long = task_1_is_last_press_long();

        // Update statistics under mutex
        xSemaphoreTake(stats_mutex, portMAX_DELAY);
        total_count++;
        if (is_long) {
            long_count++;
            total_long_duration += dur;
        } else {
            short_count++;
            total_short_duration += dur;
        }
        xSemaphoreGive(stats_mutex);

        // Blink yellow LED: 5 blinks for short, 10 for long
        int blinks = is_long ? TASK_2_BLINK_LONG : TASK_2_BLINK_SHORT;
        for (int i = 0; i < blinks * 2; i++) {
            yellow_led.toggle();
            vTaskDelay(pdMS_TO_TICKS(TASK_2_BLINK_DELAY_MS));
        }
        yellow_led.off();
    }
}
