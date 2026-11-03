#include"game_mechanics.h"
#include"game_core.h"

Clock *clock;
MathProblem m_problem;
SDL_Texture *chars_tex[C_COUNT][CS_COUNT];
const char* const chars_texPaths[C_COUNT][CS_COUNT] = CHARACTER_TEX_PATHS;

void GameMechanics_Init()
{
    size_t i, j;

    clock = Clock_Create();
    Clock_TimeSet(clock, 15, 0, 0);
    Clock_DateSet(clock, 1, JUN_, 2046);
    for(i=0; i < C_COUNT; ++i){
        for(j=0; j < CS_COUNT; ++j){
            chars_tex[i][j] = IMG_LoadTexture(r, chars_texPaths[i][j]);
        }
    }
}

void GameMechanics_Quit()
{
    size_t i, j;

    for(i=0; i < C_COUNT; ++i){
        for(j=0; j < CS_COUNT; ++j){
            SDL_DestroyTexture(chars_tex[i][j]);
        }
    }
}
