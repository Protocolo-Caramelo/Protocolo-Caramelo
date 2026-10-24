#ifndef INP_BAR_H
#define INP_BAR_H

#include<graphical.h>
#include"inputs_communs.h"

typedef struct BarInput BarInput;
struct BarInput {
    int state[INP_STATES_MAX];
    TextConfig* state_tc[INP_STATES_MAX];

    double rg_min, rg_max;
    int bar_w, bar_h;
    int w, h;

    char *fmt;

    SDL_Color c_init, c_end;
    Text *text;
};

BarInput* BarInput_Create(
    TextCache*, TextConfig* (*)[INP_STATES_MAX],
    char *fmt,
    double min, double max,
    int w, int h,
    SDL_Color c_init, SDL_Color c_end
);
void BarInput_Destroy(BarInput**);
void BarInput_Display(BarInput*, int x, int y, void(*)(BarInput*));
//
double BarInput_GetVal(BarInput*);
void BarInput_SetVaL(BarInput*bi, int val);
//
void BarInput_handleMouseButton(BarInput*bi, SDL_Event*e, void(*)(BarInput*, SDL_Event*));
void BarInput_handleMouseMotion(BarInput*bi, SDL_Event*e, void(*)(BarInput*, SDL_Event*));
void BarInput_handleInput(BarInput*bi, SDL_Event *e);

#endif
