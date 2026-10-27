#ifndef INPUTS_COMMUNS_H
#define INPUTS_COMMUNS_H

#include<math.h>

#define DEG_TO_RAD(deg) (deg * M_PI / 180)
#define RAD_TO_DEG(rad) (rad * 180 / M_PI)

typedef enum Input_State INP_State;
enum Input_State {
    INP_STATE_NEUTRAL,
    INP_STATE_SELECTED,
    INP_STATE_HOVER,
    INP_STATES_MAX
};

#endif
