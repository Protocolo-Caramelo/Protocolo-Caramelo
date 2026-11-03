#ifndef SCRN_PAUSE_H
#define SCRN_PAUSE_H

#include<SDL2/SDL.h>

void Pause_displayButtons();
void Pause_displayGameTitle();
void Pause_displayBackground();

void Pause_handleInput(SDL_Event*);

void Pause_Render();
void Pause_Load(SDL_Rect*);
void Pause_QuitToGame();
void Pause_QuitToMenu();

#endif
