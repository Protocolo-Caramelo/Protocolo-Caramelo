#ifndef SCRN_MENU_H
#define SCRN_MENU_H

#include<graphical.h>
#include"inputs/inputs.h"

void Menu_displayButtonCentered(Button*b, int x, int y);
void Menu_handleEvent(SDL_Event*);
void Menu_displayMainButtons();

void Menu_Render();

#endif
