#include "lw4_1.h"
#include "task_1_sensor_acq.h"
#include "task_2_lcd_report.h"
#include "srv_lcd_stdio.h"

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <stdio.h>

// --- Recurrences and offsets (ms) ---

#define OFFS_SENSOR    0
#define REC_SENSOR     2500    // DHT11 requires minimum 2s between reads

#define OFFS_REPORT    50
#define REC_REPORT     3000    // 3s LCD refresh (no point to do it faster)

// --- Task priorities ---

#define PRIO_SENSOR    2
#define PRIO_REPORT    1

// --- Stack sizes ---

#define STACK_SENSOR   512
#define STACK_REPORT   512

// --- FreeRTOS task wrappers ---

static void task_sensor_rtos(void* pvParameters) {
    const TickType_t offsetTicks = pdMS_TO_TICKS(OFFS_SENSOR);
    const TickType_t recTicks    = pdMS_TO_TICKS(REC_SENSOR);
    TickType_t xLastWakeTime;

    task_1_sensor_acq_setup();

    if (offsetTicks > 0) vTaskDelay(offsetTicks);
    xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        task_1_sensor_acq_loop();
        vTaskDelayUntil(&xLastWakeTime, recTicks);
    }
}

static void task_report_rtos(void* pvParameters) {
    const TickType_t offsetTicks = pdMS_TO_TICKS(OFFS_REPORT);
    const TickType_t recTicks    = pdMS_TO_TICKS(REC_REPORT);
    TickType_t xLastWakeTime;

    task_2_lcd_report_setup();

    if (offsetTicks > 0) vTaskDelay(offsetTicks);
    xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        task_2_lcd_report_loop();
        vTaskDelayUntil(&xLastWakeTime, recTicks);
    }
}

// --- Application entry points ---

void lw4_1_setup(void) {
    xTaskCreate(task_sensor_rtos, "Sensor",  STACK_SENSOR, NULL, PRIO_SENSOR, NULL);
    xTaskCreate(task_report_rtos, "Report",  STACK_REPORT, NULL, PRIO_REPORT, NULL);

    vTaskStartScheduler();
}

void lw4_1_loop(void) {
    // FreeRTOS manages all tasks; this should never execute
}
