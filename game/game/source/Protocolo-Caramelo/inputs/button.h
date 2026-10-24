#ifndef INP_BUTTON_H
#define INP_BUTTON_H

#include<graphical.h>
#include"inputs_communs.h"

typedef struct Button Button;
struct Button {
    int state[INP_STATES_MAX];
    TextConfig *state_tc[INP_STATES_MAX];

    int x, y;

    Text *text;
};

Button* Button_Create(
    TextCache*, TextConfig* (*)[INP_STATES_MAX],
    char *str
    );
void Button_Destroy(Button**);
//
void Button_Display(Button*, int x, int y, void(*func)(Button*));
//
void Button_handleMouseButton(Button*, SDL_Event*, void(*func)(Button*, SDL_Event*));
void Button_handleMouseMotion(Button*, SDL_Event*, void(*func)(Button*, SDL_Event*));
void Button_handleInput(Button*, SDL_Event*);

#endif
