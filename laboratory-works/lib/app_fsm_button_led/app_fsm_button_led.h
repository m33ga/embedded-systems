#ifndef APP_FSM_BUTTON_LED_H
#define APP_FSM_BUTTON_LED_H

#define LED_OFF_STATE 0
#define LED_ON_STATE  1
#define FSM_NUM_STATES 2
#define FSM_NUM_INPUTS 2

struct FSMState {
    unsigned long out;
    unsigned long time;
    unsigned long next[FSM_NUM_INPUTS];
};

typedef const struct FSMState STyp;

void fsmButtonLedInit(void);
void fsmButtonLedEvaluate(int input);
int  fsmButtonLedGetOutput(void);
int  fsmButtonLedGetStateIndex(void);
const char* fsmButtonLedGetStateName(void);

#endif
