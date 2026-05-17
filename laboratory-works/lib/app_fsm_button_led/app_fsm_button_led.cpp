#include "app_fsm_button_led.h"

static STyp FSM[FSM_NUM_STATES] = {
    // LED_OFF_STATE: output=0, delay=100ms, next={OFF, ON}
    {0, 10, {LED_OFF_STATE, LED_ON_STATE}},
    // LED_ON_STATE:  output=1, delay=100ms, next={ON, OFF}
    {1, 10, {LED_ON_STATE, LED_OFF_STATE}}
};

static int currentState = LED_OFF_STATE;

static const char* stateNames[FSM_NUM_STATES] = {
    "OFF",
    "ON"
};

void fsmButtonLedInit(void) {
    currentState = LED_OFF_STATE;
}

void fsmButtonLedEvaluate(int input) {
    int idx = (input != 0) ? 1 : 0;
    currentState = FSM[currentState].next[idx];
}

int fsmButtonLedGetOutput(void) {
    return FSM[currentState].out;
}

int fsmButtonLedGetStateIndex(void) {
    return currentState;
}

const char* fsmButtonLedGetStateName(void) {
    return stateNames[currentState];
}
