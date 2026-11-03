#ifndef GAME_GLOBALS_PLAY_H
#define GAME_GLOBALS_PLAY_H

#include"inputs/inputs.h"

// Textures
#define GPLAY_TEX_W 2344
#define GPLAY_TEX_H 1465

typedef enum GamePlay_Text    GamePlay_Text;
typedef enum GamePlay_Texture GamePlay_Texture;
typedef enum GamePlay_Button  GamePlay_Button;
enum GamePlay_Text {
    GPLAY_TEXT_ROOM_CLOCK_DATE,
    GPLAY_TEXT_ROOM_CLOCK_TIME,

    GPLAY_TEXT_MONITOR_CLOCK_DATE,
    GPLAY_TEXT_MONITOR_CLOCK_TIME,

    GPLAY_TEXT_PAUSED_TITLE_PROTOCOLO,
    GPLAY_TEXT_PAUSED_TITLE_CARAMELO,

    GPLAY_TEXT_COUNT
};
enum GamePlay_Texture {
    GPLAY_TEX_ROOM_1,
    GPLAY_TEX_ROOM_2,

    GPLAY_TEX_MONITOR_POWER_BUTTON_UNSELECTED,
    GPLAY_TEX_MONITOR_POWER_BUTTON_SELECTED,

    GPLAY_TEX_PAUSED,

    GPLAY_TEX_COUNT
};
enum GamePlay_Button {
    GPLAY_BUTTON_ARROW_ROOM1_BED,
    GPLAY_BUTTON_ARROW_ROOM1_BACK,
    GPLAY_BUTTON_ARROW_ROOM1_MONITOR,

    GPLAY_BUTTON_ARROW_ROOM2_BACK,
    GPLAY_BUTTON_ARROW_ROOM2_DOOR,

    GPLAY_BUTTON_MONITOR_BACK,

    GPLAY_BUTTON_PAUSED_RESUME,
    GPLAY_BUTTON_PAUSED_LOAD,
    GPLAY_BUTTON_PAUSED_CONFIG,
    GPLAY_BUTTON_PAUSED_QUIT,

    GPLAY_BUTTON_COUNT
};

extern GamePlay_Texture gamePlay_tex;
extern SDL_Texture  *play_tex[GPLAY_TEX_COUNT];
#define GPLAY_TEX_PATHS \
    { \
        [GPLAY_TEX_ROOM_1]                           = "./assets/images/scene/Room_view1.png",               \
        [GPLAY_TEX_ROOM_2]                           = "./assets/images/scene/Room_view2.png",               \
        [GPLAY_TEX_MONITOR_POWER_BUTTON_UNSELECTED]  = "./assets/images/scene/Monitor_view_unselected.png",  \
        [GPLAY_TEX_MONITOR_POWER_BUTTON_SELECTED]    = "./assets/images/scene/Monitor_view_selected.png",    \
    }

//
// Text
//
extern Text *play_text[GPLAY_TEXT_COUNT];
#define GPLAY_TEXT \
    {   \
        [GPLAY_TEXT_ROOM_CLOCK_DATE] = Text_Create(     \
            cache[GFONT_DIGITAL],                       \
            TextConfig_Create(                          \
                .scale_width  = 1 / 2.0,                \
                .scale_height = 1 / 2.0,                \
                .color    = (SDL_Color){255, 0, 0, 255},\
                .padding  = (Padding){                  \
                    .top    = 5,                        \
                    .bottom = 5,                        \
                    .color  = (SDL_Color){0, 0, 0, 200},\
                } \
            ) \
        ), \
        [GPLAY_TEXT_ROOM_CLOCK_TIME] = Text_Create(     \
            cache[GFONT_DIGITAL],                       \
            TextConfig_Create(                          \
                .scale_width  = 1 / 1.0,                \
                .scale_height = 1 / 1.0,                \
                .color    = (SDL_Color){255, 0, 0, 255},\
                .padding  = (Padding){                  \
                    .right = 5,                         \
                    .left  = 5,                         \
                    .color = (SDL_Color){0, 0, 0, 200}, \
                } \
            ) \
        ), \
        \
        [GPLAY_TEXT_MONITOR_CLOCK_DATE] = Text_Create(  \
            cache[GFONT_DIGITAL],                       \
            TextConfig_Create(                          \
                .scale_width  = 1 / 6.0,                \
                .scale_height = 1 / 6.0,                \
                .color = (SDL_Color){255, 0, 0, 255},   \
            ) \
        ), \
        [GPLAY_TEXT_MONITOR_CLOCK_TIME] = Text_Create(  \
            cache[GFONT_DIGITAL],                       \
            TextConfig_Create(                          \
                .scale_width  = 1 / 7.0,                \
                .scale_height = 1 / 7.0,                \
                .color = (SDL_Color){255, 0, 0, 255},   \
            ) \
        ), \
        \
        [GPLAY_TEXT_PAUSED_TITLE_PROTOCOLO] = Text_Create( \
            cache[GFONT_3D],                              \
            MENU_TITLE_CONFIG,                            \
            "Protocolo"                                   \
        ), \
        [GPLAY_TEXT_PAUSED_TITLE_CARAMELO] = Text_Create(  \
            cache[GFONT_3D],                              \
            MENU_TITLE_CONFIG,                            \
            "C.A.R.A.M.E.L.O"                             \
        ), \
    }


//
//  Buttons
//
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

#define GPLAY_BUTTONS \
    { \
        [GPLAY_BUTTON_ARROW_ROOM1_BACK]    = Button_Create(cache[GFONT_REGULAR], &((TextConfig*[INP_STATES_MAX])GPLAY_BUTTON_ARROW_CONFIG), "Virar"),      \
        [GPLAY_BUTTON_ARROW_ROOM1_MONITOR] = Button_Create(cache[GFONT_REGULAR], &((TextConfig*[INP_STATES_MAX])GPLAY_BUTTON_ARROW_CONFIG), "PC"),         \
        [GPLAY_BUTTON_ARROW_ROOM1_BED]     = Button_Create(cache[GFONT_REGULAR], &((TextConfig*[INP_STATES_MAX])GPLAY_BUTTON_ARROW_CONFIG), "Cama"),       \
        \
        [GPLAY_BUTTON_ARROW_ROOM2_BACK]    = Button_Create(cache[GFONT_REGULAR], &((TextConfig*[INP_STATES_MAX])GPLAY_BUTTON_ARROW_CONFIG), "Virar"),      \
        [GPLAY_BUTTON_ARROW_ROOM2_DOOR]    = Button_Create(cache[GFONT_REGULAR], &((TextConfig*[INP_STATES_MAX])GPLAY_BUTTON_ARROW_CONFIG), "Porta"),      \
        \
        [GPLAY_BUTTON_MONITOR_BACK]        = Button_Create(cache[GFONT_REGULAR], &((TextConfig*[INP_STATES_MAX])GPLAY_BUTTON_MONITOR_POWER_CONFIG), ""),   \
        \
        [GPLAY_BUTTON_PAUSED_RESUME]        = Button_Create(cache[GFONT_REGULAR], &((TextConfig*[INP_STATES_MAX])MENU_BUTTON_CONFIG), "Retornar"),         \
        [GPLAY_BUTTON_PAUSED_LOAD]          = Button_Create(cache[GFONT_REGULAR], &((TextConfig*[INP_STATES_MAX])MENU_BUTTON_CONFIG), "Carregar"),         \
        [GPLAY_BUTTON_PAUSED_CONFIG]        = Button_Create(cache[GFONT_REGULAR], &((TextConfig*[INP_STATES_MAX])MENU_BUTTON_CONFIG), "Configurar"),       \
        [GPLAY_BUTTON_PAUSED_QUIT]          = Button_Create(cache[GFONT_REGULAR], &((TextConfig*[INP_STATES_MAX])MENU_BUTTON_CONFIG), "Sair"),             \
    }

//
// Monitor
//
extern TextConfig *play_buttonMonitorPowerConfig[INP_STATES_MAX];
extern Button *play_buttons[GPLAY_BUTTON_COUNT];
#define GPLAY_BUTTON_MONITOR_POWER_CONFIG_NEUTRAL  \
    TextConfig_Create(                          \
        .bx_width   = 473,                      \
        .bx_height  = 177,                      \
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
