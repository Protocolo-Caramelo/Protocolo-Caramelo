#ifndef SCRN_BED_H
#define SCRN_BED_H

#include<SDL2/SDL.h>

void Bed_displayButtons();
void Bed_displayBackground();
void Bed_handleKeyDown();

void Bed_handleInput(SDL_Event*);

void Bed_Render();
void Bed_Load(SDL_Rect*);
void Bed_QuitToGame();
void Bed_QuitToMenu();

#endif
