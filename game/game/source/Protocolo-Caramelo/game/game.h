#ifndef GAME_H
#define GAME_H

#include<graphical.h>
#include"game_globals.h"

// Initialize all Elements
void Game_Init();
void Game_Quit();

// Play methods
void Game_PlayInit();
void Game_PlayQuit();
void Game_PlayDisplayBackground(GamePlay_Texture, SDL_Rect*, SDL_Rect*);

// Devquote(Debug)
void Game_Debug();

#endif
