#ifndef TASK_1_BTN_DUR_DET_H
#define TASK_1_BTN_DUR_DET_H

#include <Arduino_FreeRTOS.h>
#include <semphr.h>

void task_1_btn_dur_det_setup(void);
void task_1_btn_dur_det_loop(void);

// Binary semaphore signaled on each valid press-release cycle
extern SemaphoreHandle_t task_1_detected_semphr;

// Mutex-protected getters for last press data
unsigned long task_1_get_last_duration(void);
int task_1_is_last_press_long(void);

#endif
