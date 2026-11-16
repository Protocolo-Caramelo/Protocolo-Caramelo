#include"game_infos.h"
#include"game_core.h"

double i_caramelos;
double i_health;
double i_energy;
double i_tired;


void GameInfos_Init()
{
}

void GameInfos_Quit()
{
}

void GameInfos_Load(char*fpath)
{
}

void GameInfos_LoadNewGame(){
    i_caramelos = 0;
    i_health    = 100;
    i_energy    = 100;
    i_tired     = 0;
    /*
    i_health    = 100;
    i_energy    = 25;
    i_tired     = 0;
    // */
}

void GameInfos_Save()
{
}
