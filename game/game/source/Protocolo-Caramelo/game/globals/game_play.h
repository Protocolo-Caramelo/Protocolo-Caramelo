#ifndef GAME_GLOBALS_PLAY_H
#define GAME_GLOBALS_PLAY_H

#include"inputs/inputs.h"

// Textures
typedef enum GamePlay_Texture GamePlay_Texture;
typedef enum GamePlay_Button GamePlay_Button;
enum GamePlay_Texture {
    GPLAY_TEX_ROOM_1,
    GPLAY_TEX_ROOM_2,

    GPLAY_TEX_MONITOR_POWER_BUTTON_UNSELECTED,
    GPLAY_TEX_MONITOR_POWER_BUTTON_SELECTED,

    GPLAY_TEX_COUNT
};
enum GamePlay_Button {
    GPLAY_BUTTON_ARROW_ROOM1_BED,
    GPLAY_BUTTON_ARROW_ROOM1_BACK,
    GPLAY_BUTTON_ARROW_ROOM1_MONITOR,

    GPLAY_BUTTON_ARROW_ROOM2_BACK,
    GPLAY_BUTTON_ARROW_ROOM2_DOOR,

    GPLAY_BUTTON_MONITOR_BACK,

    GPLAY_BUTTON_COUNT
};

extern GamePlay_Texture gamePlay_tex;
extern SDL_Texture  *play_tex[GPLAY_TEX_COUNT];
#define GPLAY_TEX_PATHS \
    { \
        [GPLAY_TEX_ROOM_1]                           = "./assets/images/scene/Room_view1.jpg",               \
        [GPLAY_TEX_ROOM_2]                           = "./assets/images/scene/Room_view2.jpg",               \
        [GPLAY_TEX_MONITOR_POWER_BUTTON_UNSELECTED]  = "./assets/images/scene/Monitor_view_unselected.png",  \
        [GPLAY_TEX_MONITOR_POWER_BUTTON_SELECTED]    = "./assets/images/scene/Monitor_view_selected.png",    \
    }

//  Buttons
extern TextConfig *play_buttonsArrowConfig[INP_STATES_MAX];
#define GPLAY_BUTTON_ARROW_TEXTURE_SCALE (1 / 1.5)
#define GPLAY_BUTTON_ARROW_CONFIG_NEUTRAL    \
    TextConfig_Create(                          \
        .scale_width = 1 / 5.5,                 \
        .scale_height = 1 / 5.5,                \
        .color = (SDL_Color){0, 0, 0, 255},     \
        .border = (Border){                     \
            .bottom = (b_arrowState[GBUTTON_ARROW_STATE_NEUTRAL].h * GPLAY_BUTTON_ARROW_TEXTURE_SCALE * GPLAY_BUTTON_ARROW_TEXTURE_SCALE), \
        },                                      \
        .padding = (Padding){2, 2, 2, 2, (SDL_Color){}}, \
        .func_alterBorder = &gamePlay_Button_Arrow_Neutral_alterBorder \
    )
#define GPLAY_BUTTON_ARROW_CONFIG_SELECTED   \
    TextConfig_Create(                          \
        .scale_width = 1 / 5.5,                 \
        .scale_height = 1 / 5.5,                \
        .color = (SDL_Color){0, 0, 0, 255},     \
        .border = (Border){                     \
            .bottom = (b_arrowState[GBUTTON_ARROW_STATE_NEUTRAL].h * GPLAY_BUTTON_ARROW_TEXTURE_SCALE * GPLAY_BUTTON_ARROW_TEXTURE_SCALE), \
        },                                      \
        .padding = (Padding){2, 2, 2, 2, (SDL_Color){}}, \
        .func_alterBorder = &gamePlay_Button_Arrow_Selected_alterBorder, \
    )
#define GPLAY_BUTTON_ARROW_CONFIG_HOVER      \
    TextConfig_Create(                          \
        .scale_width = 1 / 5.5,                 \
        .scale_height = 1 / 5.5,                \
        .color = (SDL_Color){0, 0, 0, 255},     \
        .border = (Border){                     \
            .bottom = (b_arrowState[GBUTTON_ARROW_STATE_NEUTRAL].h * GPLAY_BUTTON_ARROW_TEXTURE_SCALE * GPLAY_BUTTON_ARROW_TEXTURE_SCALE), \
        },                                      \
        .padding = (Padding){2, 2, 2, 2, (SDL_Color){}}, \
        .func_alterBorder = &gamePlay_Button_Arrow_Hover_alterBorder \
    )
#define GPLAY_BUTTON_ARROW_CONFIG \
    { \
        [INP_STATE_NEUTRAL]  = GPLAY_BUTTON_ARROW_CONFIG_NEUTRAL,   \
        [INP_STATE_SELECTED] = GPLAY_BUTTON_ARROW_CONFIG_SELECTED,  \
        [INP_STATE_HOVER]    = GPLAY_BUTTON_ARROW_CONFIG_HOVER,     \
    }

// Monitor
extern TextConfig *play_buttonMonitorPowerConfig[INP_STATES_MAX];
extern Button *play_buttons[GPLAY_BUTTON_COUNT];
#define GPLAY_BUTTON_MONITOR_POWER_CONFIG_NEUTRAL  \
    TextConfig_Create(                          \
        .bx_width   = 510,                      \
        .bx_height  = 210,                      \
    )                                           
#define GPLAY_BUTTON_MONITOR_POWER_CONFIG_SELECTED \
    NULL
#define GPLAY_BUTTON_MONITOR_POWER_CONFIG_HOVER    \
    NULL
#define GPLAY_BUTTON_MONITOR_POWER_CONFIG \
    { \
        [INP_STATE_NEUTRAL]  = GPLAY_BUTTON_MONITOR_POWER_CONFIG_NEUTRAL,   \
        [INP_STATE_SELECTED] = GPLAY_BUTTON_MONITOR_POWER_CONFIG_SELECTED,  \
        [INP_STATE_HOVER]    = GPLAY_BUTTON_MONITOR_POWER_CONFIG_HOVER,     \
    }

void GamePlay_Init();
void GamePlay_Quit();

void gamePlay_Button_Arrow_Neutral_alterBorder(Text*, Texture*);
void gamePlay_Button_Arrow_Selected_alterBorder(Text*, Texture*);
void gamePlay_Button_Arrow_Hover_alterBorder(Text*, Texture*);

#endif
