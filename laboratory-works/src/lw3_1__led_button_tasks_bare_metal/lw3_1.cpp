#include "lw3_1.h"
#include "task_1_btn_dur_det.h"
#include "task_2_btn_stats.h"
#include "task_3_btn_report.h"
#include "srv_serial_stdio.h"

#include <Arduino.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

// --- Task context structure ---

typedef struct {
    void (*task_func)(void);
    int rec;
    int offset;
    int rec_cnt;
} Task_t;

// --- Task IDs ---

enum {
    TASK_DUR_DET_ID = 0,
    TASK_STATS_ID,
    TASK_REPORT_ID,
    MAX_OF_TASKS
};

// --- Recurrences and offsets (ms) ---

#define REC_DUR_DET   10
#define REC_STATS     100
#define REC_REPORT    10000

#define OFFS_DUR_DET  0
#define OFFS_STATS    5
#define OFFS_REPORT   50

// --- Task table ---

static Task_t tasks[MAX_OF_TASKS] = {
    { task_1_btn_dur_det_loop, REC_DUR_DET,  OFFS_DUR_DET,  0 },
    { task_2_btn_stats_loop,   REC_STATS,    OFFS_STATS,    0 },
    { task_3_btn_report_loop,  REC_REPORT,   OFFS_REPORT,   0 }
};

// --- Scheduler ---

static void os_seq_scheduler_task_init(Task_t *task, void (*func)(void),
                                       int rec, int offset) {
    task->task_func = func;
    task->rec       = rec;
    task->offset    = offset;
    task->rec_cnt   = offset;
}

static void os_seq_scheduler_setup(void) {
    os_seq_scheduler_task_init(&tasks[TASK_DUR_DET_ID],
                               task_1_btn_dur_det_loop, REC_DUR_DET, OFFS_DUR_DET);
    os_seq_scheduler_task_init(&tasks[TASK_STATS_ID],
                               task_2_btn_stats_loop, REC_STATS, OFFS_STATS);
    os_seq_scheduler_task_init(&tasks[TASK_REPORT_ID],
                               task_3_btn_report_loop, REC_REPORT, OFFS_REPORT);
}

static void os_seq_scheduler_loop(void) {
    for (int i = 0; i < MAX_OF_TASKS; i++) {
        if (--tasks[i].rec_cnt <= 0) {
            tasks[i].rec_cnt = tasks[i].rec;
            tasks[i].task_func();
        }
    }
}

// --- Timer 1: 1 ms tick ---

static volatile unsigned long os_tick = 0;

ISR(TIMER1_COMPA_vect) {
    os_tick++;
}

static void timer1_init(void) {
    TCCR1A = 0;
    TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10); // CTC, prescaler 64
    OCR1A  = 249;                                        // 16 MHz / 64 / 250 = 1 kHz
    TIMSK1 = (1 << OCIE1A);
}

// --- Application entry points ---

void lw3_1_setup(void) {
    srvSerialSetup();
    task_1_btn_dur_det_setup();
    task_2_btn_stats_setup();
    task_3_btn_report_setup();

    os_seq_scheduler_setup();
    timer1_init();
    sei();

    printf("LW3.1 Bare-metal scheduler started.\n");
    printf("Button: pin 22 | Green: pin 24 | Red: pin 26 | Yellow: pin 28\n");
}

void lw3_1_loop(void) {
    static unsigned long last_tick = 0;

    noInterrupts();
    unsigned long current_tick = os_tick;
    interrupts();

    while (last_tick < current_tick) {
        last_tick++;
        os_seq_scheduler_loop();
    }
}
