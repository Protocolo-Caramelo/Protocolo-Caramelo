#include"game_buttons.h"
#include"game_core.h"

SDL_Rect b_arrowState[GBUTTON_ARROW_STATE_COUNT];
SDL_Texture *b_arrowTex;

void GameButtons_Init()
{
    b_arrowTex = IMG_LoadTexture(r, GBUTTON_ARROW_TEXTURE_PATH);
    if(!b_arrowTex){
        printf("Error while loading button arrow texture: %s\n", SDL_GetError());
    }
    b_arrowState[GBUTTON_ARROW_STATE_NEUTRAL] = (SDL_Rect){
        .x = 108,
        .y = 64,
        .w = 94,
        .h = 135
    };
    b_arrowState[GBUTTON_ARROW_STATE_SELECTED] = (SDL_Rect){
        .x = 269,
        .y = 64,
        .w = 94,
        .h = 135
    };
    b_arrowState[GBUTTON_ARROW_STATE_HOVER] = b_arrowState[GBUTTON_ARROW_STATE_SELECTED];
}

void GameButtons_Quit()
{
    SDL_DestroyTexture(b_arrowTex);
}
