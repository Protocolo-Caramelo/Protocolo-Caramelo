#include"game_globals.h"

void game_globals_init()
{
    GameCore_Init();
    GameCache_Init();
    GameButtons_Init();
    GameState_Init();
    //
    GameMenu_Init();
    GamePlay_Init();
    GameMechanics_Init();
    //
    GameDebug_Init();

}

void game_globals_quit()
{
    GameDebug_Quit();
    //
    GameMechanics_Quit();
    GamePlay_Quit();
    GameMenu_Quit();
    //
    GameState_Quit();
    GameButtons_Quit();
    GameCache_Quit();
    GameCore_Quit();
}
