/*
 * The files monitor.c, monitor.h and module monitor has maded with help of Claude
 */
#ifndef SCRN_MONITOR_H
#define SCRN_MONITOR_H

#include"inputs/inputs.h"
#include"game/game.h"

// Life cycle (called by Game_Init / Game_Quit)
void Monitor_Init();
void Monitor_Quit();

void Monitor_handleInput(SDL_Event*e);

void Monitor_displayButtonPower(Button*b, int, int);
void Monitor_displayButtons();

void Monitor_Render();

#endif
