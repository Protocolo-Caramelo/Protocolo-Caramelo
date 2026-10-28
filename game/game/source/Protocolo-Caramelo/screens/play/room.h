#ifndef SCRN_ROOM
#define SCRN_ROOM

#include"inputs/inputs.h"
#include"game/game.h"

void Room_displayButtonArrow(Button*b, int x, int y);

//
void Room1_handleInput(SDL_Event*);
void Room1_displayButtons();
void Room1_Render();

//
void Room2_handlerInput(SDL_Event*);
void Room2_displayButtons();
void Room2_Render();

#endif
