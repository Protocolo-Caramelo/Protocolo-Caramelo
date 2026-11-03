/*
 * The files monitor.c, monitor.h and module monitor has maded with help of Claude
 */
#ifndef SCRN_MONITOR_H
#define SCRN_MONITOR_H

#include"inputs/inputs.h"

void Monitor_handleKeyDown(SDL_Event *e);
void Monitor_handleInput(SDL_Event*e);

void Monitor_displayButtonPower(Button*b, int, int);
void Monitor_displayButtons();

void Monitor_Render();
void Monitor_Load();
void Monitor_Leave();
void Monitor_Quit();

#endif
