#include"game_state.h"

GameState game_state;
GamePlay_State game_playState;

void GameState_Init()
{
    game_state = GAME_STATE_SCREEN_MENU;
}

void GameState_Quit()
{
}
