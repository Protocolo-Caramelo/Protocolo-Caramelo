#include"game_buttons.h"
#include"game_core.h"

SDL_Rect gbuttons_rects[GBUTTON_TYPE_COUNT][GBUTTON_STATE_COUNT];
SDL_Texture *gbuttons_tex[GBUTTON_TYPE_COUNT];
static const char* const gbuttons_texPath[GBUTTON_TYPE_COUNT] =
    GBUTTON_TEX_PATH
    ;

void GameButtons_Init()
{
    size_t i;

    for(i=0; i < GBUTTON_TYPE_COUNT; ++i){
        gbuttons_tex[i] = IMG_LoadTexture(r, gbuttons_texPath[i]);
        if(!gbuttons_tex[i]){
            printf("Error while loading button arrow texture: %s\n", SDL_GetError());
        }
    }

    gbuttons_rects[GBUTTON_TYPE_ARROW_1][GBUTTON_STATE_NEUTRAL] = (SDL_Rect){
        .x = 108,
        .y = 64,
        .w = 94,
        .h = 135
    };
    gbuttons_rects[GBUTTON_TYPE_ARROW_1][GBUTTON_STATE_HOVER] = (SDL_Rect){
        .x = 269,
        .y = 64,
        .w = 94,
        .h = 135
    };
    gbuttons_rects[GBUTTON_TYPE_ARROW_1][GBUTTON_STATE_SELECTED] =
        gbuttons_rects[GBUTTON_TYPE_ARROW_1][GBUTTON_STATE_HOVER];
    //
    gbuttons_rects[GBUTTON_TYPE_ARROW_2][GBUTTON_STATE_NEUTRAL] = (SDL_Rect){
        .x = 37,
        .y = 46,
        .w = 167,
        .h = 140
    };
    gbuttons_rects[GBUTTON_TYPE_ARROW_2][GBUTTON_STATE_HOVER] = (SDL_Rect){
        .x = 267,
        .y = 46,
        .w = 167,
        .h = 140
    };
    gbuttons_rects[GBUTTON_TYPE_ARROW_2][GBUTTON_STATE_SELECTED] =
        gbuttons_rects[GBUTTON_TYPE_ARROW_2][GBUTTON_STATE_HOVER];
}

void GameButtons_Quit()
{
    size_t i;
    for(i=0; i < GBUTTON_TYPE_COUNT; ++i){
        SDL_DestroyTexture(gbuttons_tex[i]);
    }
}
