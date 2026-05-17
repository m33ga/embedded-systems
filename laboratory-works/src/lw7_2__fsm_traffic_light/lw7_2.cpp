#include "lw7_2.h"
#include "task_1_fsm_traffic.h"
#include "task_2_reporting.h"
#include "srv_serial_stdio.h"
#include "srv_lcd_stdio.h"

#include <Arduino.h>
#include <Arduino_FreeRTOS.h>
#include <stdio.h>

// --- Task 2 recurrence ---

#define OFFS_REPORT    0
#define REC_REPORT     500

// --- Task priorities ---

#define PRIO_FSM       2
#define PRIO_REPORT    1

// --- Stack sizes (bytes on AVR) ---

#define STACK_FSM      256
#define STACK_REPORT   512

// --- FreeRTOS task wrappers ---

static void task_1_rtos(void *pvParameters) {
    task_1_fsm_traffic_setup();
    for (;;) {
        task_1_fsm_traffic_loop();
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

void lw7_2_setup(void) {
    srvSerialSetup();
    srvLCDInit();
    srvLCDSetup();

    printf("LW7.2 FSM Traffic Light starting.\n");
    printf("NS Request: pin 2 | EW: 22,24,26 | NS: 28,30,32\n");

    xTaskCreate(task_1_rtos, "FSMTraf", STACK_FSM,    NULL, PRIO_FSM,    NULL);
    xTaskCreate(task_2_rtos, "Report",  STACK_REPORT, NULL, PRIO_REPORT, NULL);

    vTaskStartScheduler();
}

void lw7_2_loop(void) {
}
