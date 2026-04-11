#ifndef APP_FSM_TRAFFIC_LIGHT_H
#define APP_FSM_TRAFFIC_LIGHT_H

#define LIGHT_RED    0
#define LIGHT_YELLOW 1
#define LIGHT_GREEN  2

#define ST_GO_EW      0
#define ST_WAIT_EW    1
#define ST_ALL_RED_1  2
#define ST_GO_NS      3
#define ST_WAIT_NS    4
#define ST_ALL_RED_2  5

#define FSM_TL_NUM_STATES 6
#define FSM_TL_NUM_INPUTS 2

struct TrafficState {
    unsigned long outEW;
    unsigned long outNS;
    unsigned long timeMs;
    unsigned long next[FSM_TL_NUM_INPUTS];
};

typedef const struct TrafficState TLSTyp;

void fsmTrafficInit(void);
void fsmTrafficEvaluate(int input);
int  fsmTrafficGetOutputEW(void);
int  fsmTrafficGetOutputNS(void);
unsigned long fsmTrafficGetDelayMs(void);
int  fsmTrafficGetStateIndex(void);
const char* fsmTrafficGetStateName(void);
const char* fsmTrafficGetLightName(int lightValue);

#endif
