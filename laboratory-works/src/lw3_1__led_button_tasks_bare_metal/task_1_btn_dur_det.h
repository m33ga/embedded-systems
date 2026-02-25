#ifndef TASK_1_BTN_DUR_DET_H
#define TASK_1_BTN_DUR_DET_H

void task_1_btn_dur_det_setup(void);
void task_1_btn_dur_det_loop(void);

int task_1_get_detected_flag(void);
int task_1_reset_detected_flag(void);
unsigned long task_1_get_last_duration(void);
int task_1_is_last_press_long(void);

#endif
