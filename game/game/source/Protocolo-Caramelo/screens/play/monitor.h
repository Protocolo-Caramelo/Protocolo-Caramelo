#ifndef SCRN_MONITOR_H
#define SCRN_MONITOR_H

#include"inputs/inputs.h"
#include"game/game.h"

void Monitor_handleInput(SDL_Event*e);

void Monitor_displayButtonIcon(Button*b, int, int);
void Monitor_displayButtonPower(Button*b, int, int);
void Monitor_displayButtons();

void Monitor_Render();

#endif
