#include "app_fsm_traffic_light.h"

static TLSTyp FSM[FSM_TL_NUM_STATES] = {
    // ST_GO_EW:     EW=Green, NS=Red,    100ms poll, next={GoEW, WaitEW}
    {LIGHT_GREEN,  LIGHT_RED,    100,  {ST_GO_EW, ST_WAIT_EW}},
    // ST_WAIT_EW:   EW=Yellow, NS=Red,   2000ms,     next={AllRed1, AllRed1}
    {LIGHT_YELLOW, LIGHT_RED,    2000, {ST_ALL_RED_1, ST_ALL_RED_1}},
    // ST_ALL_RED_1: EW=Red, NS=Red,      500ms,      next={GoNS, GoNS}
    {LIGHT_RED,    LIGHT_RED,    500,  {ST_GO_NS, ST_GO_NS}},
    // ST_GO_NS:     EW=Red, NS=Green,    5000ms,     next={WaitNS, WaitNS}
    {LIGHT_RED,    LIGHT_GREEN,  5000, {ST_WAIT_NS, ST_WAIT_NS}},
    // ST_WAIT_NS:   EW=Red, NS=Yellow,   2000ms,     next={AllRed2, AllRed2}
    {LIGHT_RED,    LIGHT_YELLOW, 2000, {ST_ALL_RED_2, ST_ALL_RED_2}},
    // ST_ALL_RED_2: EW=Red, NS=Red,      500ms,      next={GoEW, GoEW}
    {LIGHT_RED,    LIGHT_RED,    500,  {ST_GO_EW, ST_GO_EW}},
};

static int currentState = ST_GO_EW;

static const char* stateNames[FSM_TL_NUM_STATES] = {
    "GoEW", "WaitEW", "AllRed1", "GoNS", "WaitNS", "AllRed2"
};

static const char* lightNames[3] = {"RED", "YEL", "GRN"};

void fsmTrafficInit(void) {
    currentState = ST_GO_EW;
}

void fsmTrafficEvaluate(int input) {
    int idx = (input != 0) ? 1 : 0;
    currentState = FSM[currentState].next[idx];
}

int fsmTrafficGetOutputEW(void) {
    return FSM[currentState].outEW;
}

int fsmTrafficGetOutputNS(void) {
    return FSM[currentState].outNS;
}

unsigned long fsmTrafficGetDelayMs(void) {
    return FSM[currentState].timeMs;
}

int fsmTrafficGetStateIndex(void) {
    return currentState;
}

const char* fsmTrafficGetStateName(void) {
    return stateNames[currentState];
}

const char* fsmTrafficGetLightName(int lightValue) {
    if (lightValue >= 0 && lightValue <= 2) return lightNames[lightValue];
    return "???";
}
