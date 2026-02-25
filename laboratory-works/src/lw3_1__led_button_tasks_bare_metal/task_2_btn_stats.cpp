#include "task_2_btn_stats.h"
#include "task_1_btn_dur_det.h"
#include "dd_led.h"

#define TASK_2_BLINK_SHORT (5 * 2)
#define TASK_2_BLINK_LONG  (10 * 2)

static Led yellow_led(35);

static int total_count = 0;
static int short_count = 0;
static int long_count = 0;
static unsigned long total_short_duration = 0;
static unsigned long total_long_duration = 0;
static int blink_cnt = 0;

int task_2_get_total_count(void) {
    return total_count;
}

int task_2_get_short_count(void) {
    return short_count;
}

int task_2_get_long_count(void) {
    return long_count;
}

unsigned long task_2_get_total_duration(void) {
    return total_short_duration + total_long_duration;
}

void task_2_reset_stats(void) {
    total_count = 0;
    short_count = 0;
    long_count = 0;
    total_short_duration = 0;
    total_long_duration = 0;
}

void task_2_btn_stats_setup(void) {
    task_2_reset_stats();
    blink_cnt = 0;
    yellow_led.off();
}

void task_2_btn_stats_loop(void) {
    if (task_1_get_detected_flag()) {
        total_count++;
        unsigned long dur = task_1_get_last_duration();
        int is_long = task_1_is_last_press_long();
        task_1_reset_detected_flag();

        if (is_long) {
            long_count++;
            total_long_duration += dur;
            blink_cnt = TASK_2_BLINK_LONG;
        } else {
            short_count++;
            total_short_duration += dur;
            blink_cnt = TASK_2_BLINK_SHORT;
        }
    }

    if (blink_cnt > 0) {
        blink_cnt--;
        yellow_led.toggle();
    }
}
