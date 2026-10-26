#ifndef SCRN_MENU_H
#define SCRN_MENU_H

#include"screen_communs.h"
#include"game/game.h"
#include<graphical.h>

void Menu_displayButtonCentered(Button*b, int x, int y);
void Menu_handleEvent(SDL_Event*);
void Menu_displayMainButtons();

void Menu_Render();

#endif
