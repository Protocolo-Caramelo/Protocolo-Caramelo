#ifndef PLAY_H
#define PLAY_H

#include<SDL2/SDL.h>
#include"inputs/inputs.h"
#include"game/game_globals.h"

extern int mineSession_paused;
extern int mineSession_pausedHour;
extern int mineSession_modTime;

// Display functions
void Play_DisplayBackground(GamePlay_Texture, SDL_Rect*, SDL_Rect*, double*, double*);
void Play_DisplayButtonArrow(Button*, SDL_Rect*, int, int);
void Play_DisplayStatus(SDL_Rect*, double, double);

// Manage mechanics
void Play_GetClockFormat(char*to, char*fmt);
void Play_ManageClock();

MathProblem_Type Play_GenMineSessionProblemType(Difficulty);
MathProblem_Difficulty Play_GenMineSessionProblemDiff(Difficulty);
void Play_GenMineSession();
void Play_ManageMineSession();

//
void Play_ResetButtons();
void Play_Render();

#endif
