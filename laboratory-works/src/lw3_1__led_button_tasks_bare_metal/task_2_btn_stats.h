#ifndef TASK_2_BTN_STATS_H
#define TASK_2_BTN_STATS_H

void task_2_btn_stats_setup(void);
void task_2_btn_stats_loop(void);

int task_2_get_total_count(void);
int task_2_get_short_count(void);
int task_2_get_long_count(void);
unsigned long task_2_get_total_duration(void);
void task_2_reset_stats(void);

#endif
