#include"game_mechanics.h"
#include"game_core.h"

Clock *gclock;
//
MathProblem *mineSession[MINESESSION_SIZE];
size_t mineSession_i;
//
SDL_Texture *chars_tex[C_COUNT][CS_COUNT];
static const char* const chars_texPaths[C_COUNT][CS_COUNT] = CHARACTER_TEX_PATHS;
Difficulty gdiff;

void GameMechanics_Init()
{
    size_t i, j;

    gclock = Clock_Create();
    Clock_TimeSet(
        gclock,
        CLOCK_INIT_HOUR,
        CLOCK_INIT_MIN,
        CLOCK_INIT_SEC
    );
    Clock_DateSet(
        gclock,
        CLOCK_INIT_MDAY,
        CLOCK_INIT_MONTH,
        CLOCK_INIT_YEAR
    );
    for(i=0; i < C_COUNT; ++i){
        for(j=0; j < CS_COUNT; ++j){
            chars_tex[i][j] = IMG_LoadTexture(r, chars_texPaths[i][j]);
        }
    }
    
    gdiff = GDIFF_INIT;
}

void GameMechanics_Quit()
{
    size_t i, j;

    for(i=0; i < C_COUNT; ++i){
        for(j=0; j < CS_COUNT; ++j){
            SDL_DestroyTexture(chars_tex[i][j]);
        }
    }

    for(i=0; i < MINESESSION_SIZE; ++i){
        if(!mineSession[i]){
            continue;
        }
        free(mineSession[i]);
        mineSession[i] = NULL;
    }
}
