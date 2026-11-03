#ifndef PLAY_H
#define PLAY_H

#include<SDL2/SDL.h>
#include"inputs/inputs.h"
#include"game/game_globals.h"

// Display functions
void Play_DisplayBackground(GamePlay_Texture, SDL_Rect*, SDL_Rect*, double*, double*);
void Play_DisplayButtonArrow(Button*, SDL_Rect*, int, int);

// Manage mechanics
void Play_ManageClock();
void Play_ManageProblems();

void Play_GetClockFormat(char*to, char*fmt);

//
void Play_ResetButtons();
void Play_Render();

#endif
