#ifndef GAME_INFOS_H
#define GAME_INFOS_H

#include<SDL2/SDL.h>

extern int i_caramelos;
extern int i_health;
extern int i_energy;
extern int i_tired;
//
#define GSTATUS_TEX_SIZE 100
enum {
    GSTATUS_TEX_HEALTH,
    GSTATUS_TEX_ENERGY,
    GSTATUS_TEX_TIRED,

    GSTATUS_TEX_COUNT;
};
extern SDL_Texture *tex_status[GSTATUS_TEX_COUNT];

void GameInfos_Init();
void GameInfos_Quit();

void GameInfos_Load(char*);
void GameInfos_Save();

#endif
