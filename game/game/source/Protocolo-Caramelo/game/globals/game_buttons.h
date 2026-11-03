#ifndef GAME_GLOBALS_BUTTONS_H
#define GAME_GLOBALS_BUTTONS_H

#include"inputs/inputs.h"

// Arrow type
#define GBUTTON_TEXTURE_PATH_SEELCTED 
#define GBUTTON_TEXTURE_PATH_HOVER
#define GBUTTON_TEXTURE_PATH_NEUTRAL

#define GBUTTON_ARROW_TEXTURE_PATH  "./assets/images/button/arrow.png"
typedef enum ButtonArrow_TexState ButtonArrow_TexState;
enum ButtonArrow_TextState {
    GBUTTON_ARROW_STATE_NEUTRAL,
    GBUTTON_ARROW_STATE_SELECTED,
    GBUTTON_ARROW_STATE_HOVER,
    GBUTTON_ARROW_STATE_COUNT
};
extern SDL_Rect b_arrowState[GBUTTON_ARROW_STATE_COUNT];
extern SDL_Texture *b_arrowTex;

void GameButtons_Init();
void GameButtons_Quit();

#endif
