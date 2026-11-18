#ifndef GAME_GLOBALS_BUTTONS_H
#define GAME_GLOBALS_BUTTONS_H

#include"inputs/inputs.h"

enum ButtonType {
    GBUTTON_TYPE_ARROW_1,
    GBUTTON_TYPE_ARROW_2,

    GBUTTON_TYPE_COUNT
};
enum Button_Sates{
    GBUTTON_STATE_NEUTRAL,
    GBUTTON_STATE_SELECTED,
    GBUTTON_STATE_HOVER,

    GBUTTON_STATE_COUNT
};

#define GBUTTON_TEX_PATH \
    { \
        [GBUTTON_TYPE_ARROW_1] = "./assets/images/button/arrow.png", \
        [GBUTTON_TYPE_ARROW_2] = "./assets/images/button/arrow2.png", \
    }
extern SDL_Rect gbuttons_rects[GBUTTON_TYPE_COUNT][GBUTTON_STATE_COUNT];
extern SDL_Texture *gbuttons_tex[GBUTTON_TYPE_COUNT];

void GameButtons_Init();
void GameButtons_Quit();

#endif
