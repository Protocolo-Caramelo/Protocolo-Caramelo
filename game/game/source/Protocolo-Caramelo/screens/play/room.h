#ifndef SCRN_ROOM_H 
#define SCRN_ROOM_H

#include<SDL2/SDL.h>
#include<inputs/inputs.h>

#define ROOM_CLOCK_FMT_DATE \
    PRINT_MDAY "/" PRINT_MONTH "/" PRINT_YEAR " " PRINT_WDAY_NAME
#define ROOM_CLOCK_FMT_TIME \
    PRINT_HOUR ":" PRINT_MIN ":" PRINT_SEC

void Room_displayButtonArrow(Button*b, int x, int y);
void Room_displayBackground();
void Room_displayClock();

void Room_handleKeyDown(SDL_Event*e);
void Room_handleInput(SDL_Event*e);

//
void Room1_handleInput(SDL_Event*);
void Room1_displayButtons();
void Room1_Render();
void Room1_Load();
void Room1_Leave();
void Room1_Quit();

//
void Room2_handlerInput(SDL_Event*);
void Room2_displayButtons();
void Room2_Render();
void Room2_Load();
void Room2_Leave();
void Room2_Quit();

#endif
