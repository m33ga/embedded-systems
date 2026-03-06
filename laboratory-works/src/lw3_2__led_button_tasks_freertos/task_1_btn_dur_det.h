#ifndef TASK_1_BTN_DUR_DET_H
#define TASK_1_BTN_DUR_DET_H

#include <Arduino_FreeRTOS.h>
#include <semphr.h>

void task_1_btn_dur_det_setup(void);
void task_1_btn_dur_det_loop(void);

// Binary semaphore: signals at least one new press pending
extern SemaphoreHandle_t task_1_press_semphr;

// Mutex protecting shared pending press data
extern SemaphoreHandle_t task_1_data_mutex;

// Shared pending data (access only under task_1_data_mutex)
extern int task_1_pending_short_count;
extern int task_1_pending_long_count;
extern unsigned long task_1_pending_short_dur;
extern unsigned long task_1_pending_long_dur;
extern int task_1_last_is_long;

#endif
