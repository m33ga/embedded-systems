#include "lw5_2.h"
#include "task_1_cmd_input.h"
#include "task_2_actuator_ctrl.h"
#include "task_3_lcd_report.h"
#include "srv_serial_stdio.h"
#include "srv_lcd_stdio.h"

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <stdio.h>

// --- Recurrences and offsets (ms) ---

#define OFFS_CMD_INPUT    0
// Task 1 is blocking (getchar), no fixed recurrence

#define OFFS_ACTUATOR     10
#define REC_ACTUATOR      50

#define OFFS_REPORT       50
#define REC_REPORT        500

// --- Task priorities ---

#define PRIO_CMD_INPUT    1
#define PRIO_ACTUATOR     3
#define PRIO_REPORT       2

// --- Stack sizes ---

#define STACK_CMD_INPUT   256
#define STACK_ACTUATOR    256
#define STACK_REPORT      512

// --- FreeRTOS task wrappers ---

static void task_1_rtos(void* pvParameters) {
    const TickType_t offsetTicks = pdMS_TO_TICKS(OFFS_CMD_INPUT);

    task_1_cmd_input_setup();

    if (offsetTicks > 0) vTaskDelay(offsetTicks);

    for (;;) {
        task_1_cmd_input_loop();
    }
}

static void task_2_rtos(void* pvParameters) {
    const TickType_t offsetTicks = pdMS_TO_TICKS(OFFS_ACTUATOR);
    const TickType_t recTicks    = pdMS_TO_TICKS(REC_ACTUATOR);
    TickType_t xLastWakeTime;

    task_2_actuator_ctrl_setup();

    if (offsetTicks > 0) vTaskDelay(offsetTicks);
    xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        task_2_actuator_ctrl_loop();
        vTaskDelayUntil(&xLastWakeTime, recTicks);
    }
}

static void task_3_rtos(void* pvParameters) {
    const TickType_t offsetTicks = pdMS_TO_TICKS(OFFS_REPORT);
    const TickType_t recTicks    = pdMS_TO_TICKS(REC_REPORT);
    TickType_t xLastWakeTime;

    task_3_lcd_report_setup();

    if (offsetTicks > 0) vTaskDelay(offsetTicks);
    xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        task_3_lcd_report_loop();
        vTaskDelayUntil(&xLastWakeTime, recTicks);
    }
}

// --- Application entry points ---

void lw5_2_setup(void) {
    srvSerialSetup();
    srvLCDInit();
    srvLCDSetup();

    xTaskCreate(task_1_rtos, "CmdIn",   STACK_CMD_INPUT, NULL, PRIO_CMD_INPUT, NULL);
    xTaskCreate(task_2_rtos, "ActCtrl", STACK_ACTUATOR,  NULL, PRIO_ACTUATOR,  NULL);
    xTaskCreate(task_3_rtos, "LcdRpt",  STACK_REPORT,    NULL, PRIO_REPORT,    NULL);

    vTaskStartScheduler();
}

void lw5_2_loop(void) {
    // FreeRTOS manages all tasks
}
