#ifndef GAME_H
#define GAME_H

#include<graphical.h>
#include"game_globals.h"

// Initialize all Elements
void Game_Init();
void Game_Quit();

// Init/Load a game
void Game_PlayInit();
void Game_PlayQuit();

// Devquote(Debug)
void Game_Debug();

#endif
