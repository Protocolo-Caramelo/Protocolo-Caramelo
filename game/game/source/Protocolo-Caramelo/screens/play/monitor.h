/*
 * The files monitor.c, monitor.h and module monitor has maded with help of Claude
 */
#ifndef SCRN_MONITOR_H
#define SCRN_MONITOR_H

#include"inputs/inputs.h"

#define MONITOR_CLOCK_FMT_TIME \
    PRINT_HOUR ":" PRINT_MIN ":" PRINT_SEC
#define MONITOR_CLOCK_FMT_DATE \
    PRINT_MDAY ":" PRINT_MONTH ":" PRINT_YEAR

void Monitor_handleKeyDown(SDL_Event *e);
void Monitor_handleInput(SDL_Event*e);

void Monitor_displayButtonPower(Button*b, int, int);
void Monitor_displayButtons();
void Monitor_displayClock();

void Monitor_Render();
void Monitor_Load();
void Monitor_Leave();
void Monitor_Quit();

#endif
