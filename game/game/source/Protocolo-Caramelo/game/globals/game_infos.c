#include"game_infos.h"

int i_caramelos;
int i_health;
int i_energy;
int i_tired;
//
SDL_Texture *tex_health = NULL;
SDL_Texture *tex_energy = NULL;
SDL_Texture *tex_tired  = NULL;

void GameInfos_Init()
{
    i_caramelos = 0;
    i_health     = 0;
    i_energy    = 100;
    i_tired     = 0;
}

void GameInfos_Quit()
{
}

void GameInfos_Load(char*fpath)
{
}

void GameInfos_Save()
{
}
