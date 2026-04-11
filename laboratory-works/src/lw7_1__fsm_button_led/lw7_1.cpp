#include "lw7_1.h"
#include "task_1_fsm_eval.h"
#include "task_2_reporting.h"
#include "srv_serial_stdio.h"
#include "srv_lcd_stdio.h"

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <stdio.h>

// --- Offsets and recurrences (ms) ---

#define OFFS_FSM_EVAL  0
#define REC_FSM_EVAL   50

#define OFFS_REPORT    100
#define REC_REPORT     500

// --- Task priorities ---

#define PRIO_FSM_EVAL  2
#define PRIO_REPORT    1

// --- Stack sizes (bytes on AVR) ---

#define STACK_FSM_EVAL  192
#define STACK_REPORT    512

// --- FreeRTOS task wrappers ---

static void task_1_rtos(void *pvParameters) {
    const TickType_t offsetTicks = pdMS_TO_TICKS(OFFS_FSM_EVAL);
    const TickType_t recTicks    = pdMS_TO_TICKS(REC_FSM_EVAL);
    TickType_t xLastWakeTime;

    task_1_fsm_eval_setup();

    if (offsetTicks > 0) vTaskDelay(offsetTicks);
    xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        task_1_fsm_eval_loop();
        vTaskDelayUntil(&xLastWakeTime, recTicks);
    }
}

static void task_2_rtos(void *pvParameters) {
    const TickType_t offsetTicks = pdMS_TO_TICKS(OFFS_REPORT);
    const TickType_t recTicks    = pdMS_TO_TICKS(REC_REPORT);
    TickType_t xLastWakeTime;

    task_2_reporting_setup();

    if (offsetTicks > 0) vTaskDelay(offsetTicks);
    xLastWakeTime = xTaskGetTickCount();

    for (;;) {
        task_2_reporting_loop();
        vTaskDelayUntil(&xLastWakeTime, recTicks);
    }
}

// --- Application entry points ---

void lw7_1_setup(void) {
    srvSerialSetup();
    srvLCDInit();
    srvLCDSetup();

    printf("LW7.1 FSM Button-LED (Moore) starting.\n");
    printf("Button: pin 2 (INPUT_PULLUP) | LED: pin 13\n");

    xTaskCreate(task_1_rtos, "FSMEval",  STACK_FSM_EVAL, NULL, PRIO_FSM_EVAL, NULL);
    xTaskCreate(task_2_rtos, "Report",   STACK_REPORT,   NULL, PRIO_REPORT,   NULL);

    vTaskStartScheduler();
}

void lw7_1_loop(void) {
    // FreeRTOS manages all tasks; this should never execute
}
