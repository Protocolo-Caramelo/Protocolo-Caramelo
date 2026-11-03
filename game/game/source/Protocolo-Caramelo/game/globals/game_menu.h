#ifndef GAME_GLOBALS_MENU_H
#define GAME_GLOBALS_MENU_H

#include"inputs/inputs.h"

typedef enum MenuButtons MenuButtons;
enum MenuButtons {
    MENU_BUTTON_INIT,
    MENU_BUTTON_CONTINUE,
    MENU_BUTTON_CONFIG,
    MENU_BUTTON_ABOUT,
    MENU_BUTTON_EXIT,
    MENU_BUTTONS_MAX
};
enum Menu_Title {
    MENU_TITLE_PROTOCOLO,
    MENU_TITLE_CARAMELO,
    MENU_TITLE_COUNT,
};

extern TextConfig* menu_textConfig;
extern TextConfig* menu_buttonConfig[INP_STATES_MAX];

extern Text *menu_tTitle[MENU_TITLE_COUNT];
extern Button *menu_buttons[MENU_BUTTONS_MAX];

#define MENU_BUTTON_SPACING_TITLE_X 0
#define MENU_BUTTON_SPACING_TITLE_Y 100
#define MENU_BUTTON_SPACING_X 0
#define MENU_BUTTON_SPACING_Y 10 
#define MENU_BUTTON_SPACING_BUTTONS_BOTTOM_X 10
#define MENU_BUTTON_SPACING_BUTTONS_BOTTOM_Y 20
//
#define MENU_BUTTON_MARGIN_NEUTRAL_VAL 2
#define MENU_BUTTON_MARGIN_SELECTED_VAL 5
#define MENU_BUTTON_MARGIN_HOVER_VAL 5
//
#define MENU_BUTTON_PADDING_NEUTRAL_W 300
#define MENU_BUTTON_PADDING_NEUTRAL_H 40
//
#define MENU_BUTTON_PADDING_SELECTED_W \
    (MENU_BUTTON_PADDING_NEUTRAL_W - MENU_BUTTON_MARGIN_SELECTED_VAL + MENU_BUTTON_MARGIN_NEUTRAL_VAL)
#define MENU_BUTTON_PADDING_SELECTED_H \
    (MENU_BUTTON_PADDING_NEUTRAL_H - MENU_BUTTON_MARGIN_SELECTED_VAL + MENU_BUTTON_MARGIN_NEUTRAL_VAL)
//
#define MENU_BUTTON_PADDING_HOVER_W  \
    (MENU_BUTTON_PADDING_NEUTRAL_W - MENU_BUTTON_MARGIN_HOVER_VAL + MENU_BUTTON_MARGIN_NEUTRAL_VAL)
#define MENU_BUTTON_PADDING_HOVER_H \
    (MENU_BUTTON_PADDING_NEUTRAL_H - MENU_BUTTON_MARGIN_HOVER_VAL + MENU_BUTTON_MARGIN_NEUTRAL_VAL)

#define MENU_BUTTON_CONFIG_NEUTRAL  \
    TextConfig_Create(              \
        .scale_width = 1 / 4.0,     \
        .scale_height = 1 / 4.0,    \
        .color = (SDL_Color){       \
            .r=255,                 \
            .g=180,                 \
            .b=27,                  \
            .a=255                  \
        },                          \
        .margin = (Margin){         \
            .left   = MENU_BUTTON_MARGIN_NEUTRAL_VAL, \
            .right  = MENU_BUTTON_MARGIN_NEUTRAL_VAL, \
            .top    = MENU_BUTTON_MARGIN_NEUTRAL_VAL, \
            .bottom = MENU_BUTTON_MARGIN_NEUTRAL_VAL, \
            .color = (SDL_Color){   \
                .a=255              \
            }                       \
        },                          \
        .padding = (Padding){       \
            .color = (SDL_Color){   \
                .r=255,             \
                .g=255,             \
                .b=255,             \
                .a=255              \
            }                       \
        },                          \
    )
#define MENU_BUTTON_CONFIG_SELECTED \
        TextConfig_Create(          \
        .scale_width = 1 / 4.0,     \
        .scale_height = 1 / 4.0,    \
        .color = (SDL_Color){       \
            .r=255,                 \
            .g=180,                 \
            .b=71,                  \
            .a=255                  \
        },                          \
        .margin = (Margin){         \
            .left   = MENU_BUTTON_MARGIN_SELECTED_VAL, \
            .right  = MENU_BUTTON_MARGIN_SELECTED_VAL, \
            .top    = MENU_BUTTON_MARGIN_SELECTED_VAL, \
            .bottom = MENU_BUTTON_MARGIN_SELECTED_VAL, \
            .color = (SDL_Color){   \
                .a=255              \
            }                       \
        },                          \
        .padding = (Padding){       \
            .color = (SDL_Color){   \
                .r=255,             \
                .g=255,             \
                .b=255,             \
                .a=255              \
            }                       \
        },                          \
    );
#define MENU_BUTTON_CONFIG_HOVER \
        TextConfig_Create(          \
        .scale_width = 1 / 4.0,     \
        .scale_height = 1 / 4.0,    \
        .color = (SDL_Color){       \
            .r=251,                 \
            .g=223,                 \
            .b=71,                  \
            .a=255                  \
        },                          \
        .margin = (Margin){         \
            .left   = MENU_BUTTON_MARGIN_HOVER_VAL, \
            .right  = MENU_BUTTON_MARGIN_HOVER_VAL, \
            .top    = MENU_BUTTON_MARGIN_HOVER_VAL, \
            .bottom = MENU_BUTTON_MARGIN_HOVER_VAL, \
            .color = (SDL_Color){   \
                .a=255              \
            }                       \
        },                          \
        .padding = (Padding){       \
            .color = (SDL_Color){   \
                .r=255,             \
                .g=255,             \
                .b=255,             \
                .a=255              \
            }                       \
        },                          \
    );
//

void GameMenu_Init();
void GameMenu_Quit();

#endif
