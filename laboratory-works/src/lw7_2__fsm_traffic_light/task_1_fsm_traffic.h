#ifndef TASK_1_FSM_TRAFFIC_H
#define TASK_1_FSM_TRAFFIC_H

void task_1_fsm_traffic_setup(void);
void task_1_fsm_traffic_loop(void);

const char* task_1_get_state_name(void);
int task_1_get_output_ew(void);
int task_1_get_output_ns(void);

#endif
