#include"game_mechanics.h"
#include"game_play.h"
#include"game_core.h"

Clock *gclock;

// MineSession
MathProblem *mineSession[MINESESSION_SIZE];
size_t mineSession_i;

// Characters texture
SDL_Texture *chars_tex[C_COUNT][CS_COUNT];
static const char* const chars_texPaths[C_COUNT][CS_COUNT] = CHARACTER_TEX_PATHS;
Difficulty gdiff;

// Status
SDL_Texture *status_tex[GSTATUS_TEX_COUNT];
Text *coins_t;
static const char* const status_texPath[GSTATUS_TEX_COUNT] = 
    GSTATUS_TEX_PATH
    ;

void GameMechanics_Init()
{
    SDL_Texture **tex;
    size_t i, j;

    //
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
    
    gdiff = GDIFF_NORMAL;

    //
    for(i=0; i < GSTATUS_TEX_COUNT; ++i){
        tex = &status_tex[i];
        *tex = IMG_LoadTexture(r, status_texPath[i]);
        if(!*tex){
            printf("Error while opening image texture: %s\n", SDL_GetError());
        }
    }

    //
    coins_t = Text_Create(
        cache[GFONT_REGULAR],
        TextConfig_Create(
            .scale_width  = 1 / 4.0,
            .scale_height = 1 / 4.0,
            .color = (SDL_Color){
                .r = 255,
                .g = 180,
                .b = 27,
                .a = 255
            },
            .func_alterBorder = coins_t_alterBorder,
        )
    );

    Dialog_Init();
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

    for(i=0; i < GSTATUS_TEX_COUNT; ++i){
        SDL_DestroyTexture(status_tex[i]);
    }
    Text_Destroy(&coins_t);

    Dialog_Quit();
}

//
void coins_t_alterBorder(Text*t, Texture*tex)
{
    SDL_Texture *tex_r;
    SDL_Rect dest;

    tex_r = status_tex[GSTATUS_TEX_COINS];
    dest = (SDL_Rect){
        .x = 0,
        .y = 0,
        .w = *Text_borderLeft(t),
        .h = (*Text_edges(t))[DRAW_PADDING_HEIGHT],
    };

    SDL_RenderCopy(
        r,
        tex_r,
        NULL,
        &dest
    );
}
