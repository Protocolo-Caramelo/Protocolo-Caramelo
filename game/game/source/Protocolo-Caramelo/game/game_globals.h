#ifndef GAME_GLOBALS_H
#define GAME_GLOBALS_H

#include<graphical.h>
#include"inputs/inputs.h"
#include"mechanics/mechanics.h"

#if defined(COMPILATION_DEVQUOTE) || defined(COMPILATION_TOOLCHAIN_WINDOWS_DEVQUOTE)
#define GAME_DEBUG
#endif

void game_globals_init();
void game_globals_quit();

//
// SDL
// 
#define ELEMENT_WINDOW_NAME "Protocolo-Caramelo"
#define ELEMENT_WINDOW_X_POSITION SDL_WINDOWPOS_UNDEFINED
#define ELEMENT_WINDOW_Y_POSITION SDL_WINDOWPOS_UNDEFINED
#define ELEMENT_WINDOW_W 960
#define ELEMENT_WINDOW_H 600
#define ELEMENT_WINDOW_FLAGS (SDL_WINDOW_RESIZABLE)
extern SDL_Window *w;
//
#define ELEMENT_RENDERER_FLAGS (SDL_RENDERER_ACCELERATED)
extern SDL_Renderer *r;
//
extern SDL_Event e;

//
// Cache
//
#define ELEMENT_CACHE_FREGULAR_PATH "assets/fonts/pixeloid-font/PixeloidMono-nAOpP.ttf"
#define ELEMENT_CACHE_FREGULAR_PTSIZE 100
extern TextCache *cache_fRegular;
#define ELEMENT_CACHE_FTITLE_PATH "assets/fonts/Press_Start_2P/PressStart2P-Regular.ttf"
#define ELEMENT_CACHE_FTITLE_PTSIZE 100
extern TextCache *cache_fTitle;
#define ELEMENT_CACHE_3DFONT_1_PATH "assets/fonts/3d/ka1.ttf"
#define ELEMENT_CACHE_3DFONT_1_PTSIZE 100
extern TextCache *cache_f3D1;

//
// Buttons type
//
#define BUTTON_TEXTURE_PATH_SEELCTED 
#define BUTTON_TEXTURE_PATH_HOVER
#define BUTTON_TEXTURE_PATH_NEUTRAL

#define BUTTON_ARROW_TEXTURE_PATH  "./assets/images/button/arrow.png"
typedef enum ButtonArrow_TexState ButtonArrow_TexState;
enum ButtonArrow_TextState {
    BUTTON_ARROW_STATE_NEUTRAL,
    BUTTON_ARROW_STATE_SELECTED,
    BUTTON_ARROW_STATE_HOVER,
    BUTTON_ARROW_STATE_COUNT
};
extern SDL_Rect b_arrowState[BUTTON_ARROW_STATE_COUNT];
extern SDL_Texture *b_arrowTex;


//
// Game State
//
typedef enum GameState GameState;
enum GameState {
    GAME_STATE_SCREEN_MENU,
    GAME_STATE_SCREEN_CONFIG,
    GAME_STATE_SCREEN_ABOUT,
    GAME_STATE_SCREEN_PLAY,
};
extern GameState gameState;

typedef enum GamePlay_State GamePlay_State;
enum GamePlay_State {
    GAMEPLAY_STATE_VIEW_ROOM_1,
    GAMEPLAY_STATE_VIEW_ROOM_2,
    GAMEPLAY_STATE_VIEW_MONITOR,
};
extern GamePlay_State gamePlay_state;

//
// Menu
//
extern TextConfig* menu_textConfig;
extern TextConfig* menu_buttonConfig[INP_STATES_MAX];

enum Menu_Title {
    MENU_TITLE_PROTOCOLO,
    MENU_TITLE_CARAMELO,
    MENU_TITLE_COUNT,
};
extern Text *menu_tTitle[MENU_TITLE_COUNT];

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
typedef enum MenuButtons {
    MENU_BUTTON_INIT,
    MENU_BUTTON_CONTINUE,
    MENU_BUTTON_CONFIG,
    MENU_BUTTON_ABOUT,
    MENU_BUTTON_EXIT,
    MENU_BUTTONS_MAX
};
extern Button *menu_buttons[MENU_BUTTONS_MAX];

// Mathematic
extern MathProblem problem;

//
// Game Play Textures And Buttons
//
typedef enum GamePlay_Texture GamePlay_Texture;
enum GamePlay_Texture {
    GAMEPLAY_TEX_ROOM_1,
    GAMEPLAY_TEX_ROOM_2,
    GAMEPLAY_TEX_MONITOR,
    GAMEPLAY_TEX_COUNT
};
extern GamePlay_Texture gamePlay_tex;
extern SDL_Texture  *play_tex[GAMEPLAY_TEX_COUNT];
#define GAMEPLAY_TEX_ROOM_1_PATH    "./assets/images/scene/Room_view1.jpg"
#define GAMEPLAY_TEX_ROOM_2_PATH    "./assets/images/scene/Room_view2.jpg"
#define GAMEPLAY_TEX_MONITOR_PATH   "./assets/images/scene/Monitor_view.jpg"

typedef enum GamePlay_Button GamePlay_Button;
enum GamePlay_Button {
    GAMEPLAY_BUTTON_ARROW_ROOM1_BED,
    GAMEPLAY_BUTTON_ARROW_ROOM1_BACK,
    GAMEPLAY_BUTTON_ARROW_ROOM1_MONITOR,

    GAMEPLAY_BUTTON_ARROW_ROOM2_BACK,
    GAMEPLAY_BUTTON_ARROW_ROOM2_DOOR,

    GAMEPLAY_BUTTON_ARROW_MONITOR_BACK,
    GAMEPLAY_BUTTON_MONITOR_MINE,
    GAMEPLAY_BUTTON_MONITOR_DELIVERY,

    GAMEPLAY_BUTTON_COUNT
};
#define GAMEPLAY_BUTTON_ARROW_TEXTURE_SCALE (1 / 1.5)
#define GAMEPLAY_BUTTON_ARROW_CONFIG_NEUTRAL    \
    TextConfig_Create(                          \
        .scale_width = 1 / 5.5,                 \
        .scale_height = 1 / 5.5,                \
        .color = (SDL_Color){0, 0, 0, 255},     \
        .border = (Border){                     \
            .bottom = (b_arrowState[BUTTON_ARROW_STATE_NEUTRAL].h * GAMEPLAY_BUTTON_ARROW_TEXTURE_SCALE),       \
            .left   = (b_arrowState[BUTTON_ARROW_STATE_NEUTRAL].w * GAMEPLAY_BUTTON_ARROW_TEXTURE_SCALE) / 2,   \
            .right  = (b_arrowState[BUTTON_ARROW_STATE_NEUTRAL].w * GAMEPLAY_BUTTON_ARROW_TEXTURE_SCALE) / 2,   \
        },                                      \
        .padding = (Padding){2, 2, 2, 2, (SDL_Color){}}, \
        .func_alterBorder = &gamePlay_Button_Arrow_Neutral_alterBorder \
    )
#define GAMEPLAY_BUTTON_ARROW_CONFIG_SELECTED   \
    TextConfig_Create(                          \
        .scale_width = 1 / 5.5,                 \
        .scale_height = 1 / 5.5,                \
        .color = (SDL_Color){0, 0, 0, 255},     \
        .border = (Border){                     \
            .bottom = (b_arrowState[BUTTON_ARROW_STATE_NEUTRAL].h * GAMEPLAY_BUTTON_ARROW_TEXTURE_SCALE),       \
            .left   = (b_arrowState[BUTTON_ARROW_STATE_NEUTRAL].w * GAMEPLAY_BUTTON_ARROW_TEXTURE_SCALE) / 2,   \
            .right  = (b_arrowState[BUTTON_ARROW_STATE_NEUTRAL].w * GAMEPLAY_BUTTON_ARROW_TEXTURE_SCALE) / 2    \
        },                                      \
        .padding = (Padding){2, 2, 2, 2, (SDL_Color){}}, \
        .func_alterBorder = &gamePlay_Button_Arrow_Selected_alterBorder, \
    )
#define GAMEPLAY_BUTTON_ARROW_CONFIG_HOVER      \
    TextConfig_Create(                          \
        .scale_width = 1 / 5.5,                 \
        .scale_height = 1 / 5.5,                \
        .color = (SDL_Color){0, 0, 0, 255},     \
        .border = (Border){                     \
            .bottom = (b_arrowState[BUTTON_ARROW_STATE_NEUTRAL].h * GAMEPLAY_BUTTON_ARROW_TEXTURE_SCALE),       \
            .left   = (b_arrowState[BUTTON_ARROW_STATE_NEUTRAL].w * GAMEPLAY_BUTTON_ARROW_TEXTURE_SCALE) / 2,   \
            .right  = (b_arrowState[BUTTON_ARROW_STATE_NEUTRAL].w * GAMEPLAY_BUTTON_ARROW_TEXTURE_SCALE) / 2    \
        },                                      \
        .padding = (Padding){2, 2, 2, 2, (SDL_Color){}}, \
        .func_alterBorder = &gamePlay_Button_Arrow_Hover_alterBorder \
    )
extern TextConfig *play_buttonsArrowConfig[INP_STATES_MAX];

#define GAMEPLAY_BUTTON_MONITOR_CONFIG_NEUTRAL  \
    TextConfig_Create(                          \
        .scale_width  = 1 / 6.5,                \
        .scale_height = 1 / 6.5,                \
        .color = (SDL_Color){0, 0, 0, 255},     \
    )                                           
#define GAMEPLAY_BUTTON_MONITOR_CONFIG_SELECTED \
    TextConfig_Create(                          \
        .scale_width  = 1 / 6.5,                \
        .scale_height = 1 / 6.5,                \
        .color = (SDL_Color){0, 0, 0, 255},     \
    )                                           
#define GAMEPLAY_BUTTON_MONITOR_CONFIG_HOVER    \
    TextConfig_Create(                          \
        .scale_width  = 1 / 6.5,                \
        .scale_height = 1 / 6.5,                \
        .color = (SDL_Color){0, 0, 0, 255},     \
    )                                           
extern TextConfig *play_buttonsMonitorConfig[INP_STATES_MAX];

extern Button *play_buttons[GAMEPLAY_BUTTON_COUNT];
void gamePlay_Button_Arrow_Neutral_alterBorder(Text*, Texture*);
void gamePlay_Button_Arrow_Selected_alterBorder(Text*, Texture*);
void gamePlay_Button_Arrow_Hover_alterBorder(Text*, Texture*);

//
// Devquote(Debug)
//
#ifdef GAME_DEBUG
extern Text *fps;
extern int fps_time;
extern int fps_delayTime;
extern const int fps_delay;
extern char fps_buffer[32];
#define FPS_CONFIG              \
    TextConfig_Create(          \
        .scale_width = 1 / 4.0, \
        .scale_height = 1 / 4.0,\
        .color = (SDL_Color){   \
            .r = 255,           \
            .g = 255,           \
            .a = 255            \
        },                      \
        .color_bg = (SDL_Color){\
            .b = 255,           \
            .a = 255            \
        }                       \
    )
#endif

// Others
extern int w_width;
extern int w_height;
extern double w_ratio;
#define WINDOW_RATIO_W (ELEMENT_WINDOW_W / (double)ELEMENT_WINDOW_H)
#define WINDOW_RATIO_H (ELEMENT_WINDOW_H / (double)ELEMENT_WINDOW_W)

#define SWAP_VALUES(a, b) \
    do {            \
        a += b;     \
        b = a - b;  \
        a -= b;      \
    }while(0)

#endif
