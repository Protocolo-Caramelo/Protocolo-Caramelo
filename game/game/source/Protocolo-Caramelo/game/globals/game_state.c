#include"game_state.h"

Game_State game_state;
Game_PlayState game_playState;

void GameState_Init()
{
    game_state = GAME_STATE_SCREEN_MENU;
}

void GameState_Quit()
{
}
