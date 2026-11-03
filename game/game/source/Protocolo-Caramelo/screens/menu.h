#ifndef SCRN_MENU_H
#define SCRN_MENU_H

#include<graphical.h>
#include"inputs/inputs.h"

void Menu_displayButtonCentered(Button*b, int x, int y, int state_changed);
void Menu_handleEvent(SDL_Event*);
void Menu_displayButtons();

void Menu_Render();

#endif
