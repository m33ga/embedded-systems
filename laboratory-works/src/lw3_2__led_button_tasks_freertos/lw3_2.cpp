#include "lw3_2.h"
#include "task_1_btn_dur_det.h"
#include "task_2_btn_stats.h"
#include "task_3_btn_report.h"
#include "srv_serial_stdio.h"

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <stdio.h>

// --- Offsets and recurrences (ms) ---

#define OFFS_DUR_DET  0
#define REC_DUR_DET   20

#define OFFS_STATS    5
// Task 2 is event-driven (blocks on semaphore), no fixed recurrence

#define OFFS_REPORT   50
#define REC_REPORT    10000

// --- Task priorities (higher number = higher priority) ---

#define PRIO_DUR_DET   3
#define PRIO_STATS     2
#define PRIO_REPORT    1

// --- Stack sizes (bytes on AVR) ---

#define STACK_DUR_DET  192
#define STACK_STATS    256
#define STACK_REPORT   512

// --- FreeRTOS task wrappers ---

static void task_1_rtos(void *pvParameters) {
    const TickType_t offsetTicks = pdMS_TO_TICKS(OFFS_DUR_DET);
    const TickType_t recTicks    = pdMS_TO_TICKS(REC_DUR_DET);
    TickType_t xLastWakeTime;

    task_1_btn_dur_det_setup();

    if (offsetTicks > 0) vTaskDelay(offsetTicks);
    xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        task_1_btn_dur_det_loop();
        vTaskDelayUntil(&xLastWakeTime, recTicks);
    }
}

static void task_2_rtos(void *pvParameters) {
    const TickType_t offsetTicks = pdMS_TO_TICKS(OFFS_STATS);

    task_2_btn_stats_setup();

    if (offsetTicks > 0) vTaskDelay(offsetTicks);

    for (;;) {
        task_2_btn_stats_loop();
    }
}

static void task_3_rtos(void *pvParameters) {
    const TickType_t offsetTicks = pdMS_TO_TICKS(OFFS_REPORT);
    const TickType_t recTicks    = pdMS_TO_TICKS(REC_REPORT);
    TickType_t xLastWakeTime;

    task_3_btn_report_setup();

    if (offsetTicks > 0) vTaskDelay(offsetTicks);
    xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        task_3_btn_report_loop();
        vTaskDelayUntil(&xLastWakeTime, recTicks);
    }
}

// --- Application entry points ---

void lw3_2_setup(void) {
    srvSerialSetup();

    printf("LW3.2 FreeRTOS preemptive scheduler starting.\n");
    printf("Button: pin 22 | Green: pin 33 | Red: pin 31 | Yellow: pin 35\n");

    xTaskCreate(task_1_rtos, "BtnDet",    STACK_DUR_DET, NULL, PRIO_DUR_DET, NULL);
    xTaskCreate(task_2_rtos, "BtnStats",  STACK_STATS,   NULL, PRIO_STATS,   NULL);
    xTaskCreate(task_3_rtos, "BtnReport", STACK_REPORT,  NULL, PRIO_REPORT,  NULL);

    vTaskStartScheduler();
}

void lw3_2_loop(void) {
    // FreeRTOS manages all tasks; this should never execute
}
