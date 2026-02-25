#include "task_3_btn_report.h"
#include "task_2_btn_stats.h"
#include <stdio.h>

void task_3_btn_report_setup(void) {
    // stdio is initialized in lw3_1 setup
}

void task_3_btn_report_loop(void) {
    int total = task_2_get_total_count();
    int shorts = task_2_get_short_count();
    int longs = task_2_get_long_count();
    unsigned long total_dur = task_2_get_total_duration();
    unsigned long avg = (total > 0) ? (total_dur / total) : 0;

    printf("--- Button Press Report: ---\n");
    printf("Total presses:   %d\n", total);
    printf("Short (<500ms):  %d\n", shorts);
    printf("Long  (>=500ms): %d\n", longs);
    printf("Avg duration:    %lu ms\n", avg);
    printf("--- ---\n");

    task_2_reset_stats();
}
