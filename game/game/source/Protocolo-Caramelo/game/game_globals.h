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
// Game Play Textures
//
typedef enum GamePlay_Texture GamePlay_Texture;
enum GamePlay_Texture {
    GAMEPLAY_TEX_ROOM_1,
    GAMEPLAY_TEX_ROOM_2,

    GAMEPLAY_TEX_MONITOR_POWER_BUTTON_UNSELECTED,
    GAMEPLAY_TEX_MONITOR_POWER_BUTTON_SELECTED,

    GAMEPLAY_TEX_COUNT
};
extern GamePlay_Texture gamePlay_tex;
extern SDL_Texture  *play_tex[GAMEPLAY_TEX_COUNT];
extern char* play_texPaths[];
#define GAMEPLAY_TEX_PATHS \
    { \
        [GAMEPLAY_TEX_ROOM_1]                           = "./assets/images/scene/Room_view1.jpg",               \
        [GAMEPLAY_TEX_ROOM_2]                           = "./assets/images/scene/Room_view2.jpg",               \
        [GAMEPLAY_TEX_MONITOR_POWER_BUTTON_UNSELECTED]  = "./assets/images/scene/Monitor_view_unselected.png",  \
        [GAMEPLAY_TEX_MONITOR_POWER_BUTTON_SELECTED]    = "./assets/images/scene/Monitor_view_selected.png",    \
    }

//
// Game Play Button
//
typedef enum GamePlay_Button GamePlay_Button;
enum GamePlay_Button {
    GAMEPLAY_BUTTON_ARROW_ROOM1_BED,
    GAMEPLAY_BUTTON_ARROW_ROOM1_BACK,
    GAMEPLAY_BUTTON_ARROW_ROOM1_MONITOR,

    GAMEPLAY_BUTTON_ARROW_ROOM2_BACK,
    GAMEPLAY_BUTTON_ARROW_ROOM2_DOOR,

    GAMEPLAY_BUTTON_MONITOR_BACK,

    GAMEPLAY_BUTTON_COUNT
};
#define GAMEPLAY_BUTTON_ARROW_TEXTURE_SCALE (1 / 1.5)
#define GAMEPLAY_BUTTON_ARROW_CONFIG_NEUTRAL    \
    TextConfig_Create(                          \
        .scale_width = 1 / 5.5,                 \
        .scale_height = 1 / 5.5,                \
        .color = (SDL_Color){0, 0, 0, 255},     \
        .border = (Border){                     \
            .bottom = (b_arrowState[BUTTON_ARROW_STATE_NEUTRAL].h * GAMEPLAY_BUTTON_ARROW_TEXTURE_SCALE * GAMEPLAY_BUTTON_ARROW_TEXTURE_SCALE), \
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
            .bottom = (b_arrowState[BUTTON_ARROW_STATE_NEUTRAL].h * GAMEPLAY_BUTTON_ARROW_TEXTURE_SCALE * GAMEPLAY_BUTTON_ARROW_TEXTURE_SCALE), \
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
            .bottom = (b_arrowState[BUTTON_ARROW_STATE_NEUTRAL].h * GAMEPLAY_BUTTON_ARROW_TEXTURE_SCALE * GAMEPLAY_BUTTON_ARROW_TEXTURE_SCALE), \
        },                                      \
        .padding = (Padding){2, 2, 2, 2, (SDL_Color){}}, \
        .func_alterBorder = &gamePlay_Button_Arrow_Hover_alterBorder \
    )
extern TextConfig *play_buttonsArrowConfig[INP_STATES_MAX];

// Monitor
#define GAMEPLAY_BUTTON_MONITOR_POWER_CONFIG_NEUTRAL  \
    TextConfig_Create(                          \
        .bx_width   = 510,                      \
        .bx_height  = 210,                      \
    )                                           
#define GAMEPLAY_BUTTON_MONITOR_POWER_CONFIG_SELECTED \
    NULL
#define GAMEPLAY_BUTTON_MONITOR_POWER_CONFIG_HOVER    \
    NULL

extern TextConfig *play_buttonMonitorPowerConfig[INP_STATES_MAX];

extern Button *play_buttons[GAMEPLAY_BUTTON_COUNT];
void gamePlay_Button_Arrow_Neutral_alterBorder(Text*, Texture*);
void gamePlay_Button_Arrow_Selected_alterBorder(Text*, Texture*);
void gamePlay_Button_Arrow_Hover_alterBorder(Text*, Texture*);

//
// Characters
//
typedef enum Character Character;
typedef enum Character_State Character_State;

enum Character {
    C_PEDRO,
    C_FELIPE,
    C_ANA,
    C_JOSE,
    C_RICARDO,
    C_DONODOPREDIO,
    C_INSPETOR,

    C_COUNT
};
enum Character_State {
    CS_NORMAL,
    CS_HAPPY,
    CS_ANIMATED,
    CS_SAD,
    CS_ANGRY,
    CS_REVOLTED,
    CS_DISTRUSTING,
    CS_DARK,
    CS_SUPER_ANIMATED,

    CS_COUNT
};

extern SDL_Texture *chars_tex[C_COUNT][CS_COUNT];
#define CHARACTER_TEX_PATHS_PREFIX "./assets/images/characters"
#define CHARACTER_TEX_PATHS \
    {                           \
        [C_PEDRO][CS_NORMAL]        = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Pedro/Normal.png")                               \
        [C_PEDRO][CS_HAPPY]         = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Pedro/Happy.png")                                \
        [C_PEDRO][CS_ANIMATED]      = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Pedro/Animated.png")                             \
        [C_PEDRO][CS_SAD]           = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Pedro/Sad.png")                                  \
        [C_PEDRO][CS_ANGRY]         = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Pedro/Angry.png")                                \
        [C_PEDRO][CS_REVOLTED]      = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Pedro/Revolted.png")                             \
        [C_PEDRO][CS_DISTRUSTING]   = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Pedro/Distrusting.png")                          \
        [C_PEDRO][CS_DARK]          = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Pedro/Dark.png")                                 \
        [C_PEDRO][CS_SUPERANIMATED] = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Pedro/SuperAnimated.png")                        \
        \
        [C_FELIPE][CS_NORMAL]        = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Felipe/Normal.png")                             \
        [C_FELIPE][CS_HAPPY]         = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Felipe/Happy.png")                              \
        [C_FELIPE][CS_ANIMATED]      = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Felipe/Animated.png")                           \
        [C_FELIPE][CS_SAD]           = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Felipe/Sad.png")                                \
        [C_FELIPE][CS_ANGRY]         = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Felipe/Angry.png")                              \
        [C_FELIPE][CS_REVOLTED]      = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Felipe/Revolted.png")                           \
        [C_FELIPE][CS_DISTRUSTING]   = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Felipe/Distrusting.png")                        \
        [C_FELIPE][CS_DARK]          = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Felipe/Dark.png")                               \
        [C_FELIPE][CS_SUPERANIMATED] = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Felipe/SuperAnimated.png")                      \
        \
        [C_ANA][CS_NORMAL]        = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Ana/Normal.png")                                   \
        [C_ANA][CS_HAPPY]         = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Ana/Happy.png")                                    \
        [C_ANA][CS_ANIMATED]      = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Ana/Animated.png")                                 \
        [C_ANA][CS_SAD]           = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Ana/Sad.png")                                      \
        [C_ANA][CS_ANGRY]         = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Ana/Angry.png")                                    \
        [C_ANA][CS_REVOLTED]      = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Ana/Revolted.png")                                 \
        [C_ANA][CS_DISTRUSTING]   = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Ana/Distrusting.png")                              \
        [C_ANA][CS_DARK]          = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Ana/Dark.png")                                     \
        [C_ANA][CS_SUPERANIMATED] = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Ana/SuperAnimated.png")                            \
        \
        [C_JOSE][CS_NORMAL]        = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Jose/Normal.png")                                 \
        [C_JOSE][CS_HAPPY]         = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Jose/Happy.png")                                  \
        [C_JOSE][CS_ANIMATED]      = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Jose/Animated.png")                               \
        [C_JOSE][CS_SAD]           = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Jose/Sad.png")                                    \
        [C_JOSE][CS_ANGRY]         = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Jose/Angry.png")                                  \
        [C_JOSE][CS_REVOLTED]      = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Jose/Revolted.png")                               \
        [C_JOSE][CS_DISTRUSTING]   = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Jose/Distrusting.png")                            \
        [C_JOSE][CS_DARK]          = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Jose/Dark.png")                                   \
        [C_JOSE][CS_SUPERANIMATED] = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Jose/SuperAnimated.png")                          \
        \
        [C_RICARDO][CS_NORMAL]        = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Ricardo/Normal.png")                           \
        [C_RICARDO][CS_HAPPY]         = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Ricardo/Happy.png")                            \
        [C_RICARDO][CS_ANIMATED]      = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Ricardo/Animated.png")                         \
        [C_RICARDO][CS_SAD]           = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Ricardo/Sad.png")                              \
        [C_RICARDO][CS_ANGRY]         = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Ricardo/Angry.png")                            \
        [C_RICARDO][CS_REVOLTED]      = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Ricardo/Revolted.png")                         \
        [C_RICARDO][CS_DISTRUSTING]   = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Ricardo/Distrusting.png")                      \
        [C_RICARDO][CS_DARK]          = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Ricardo/Dark.png")                             \
        [C_RICARDO][CS_SUPERANIMATED] = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Ricardo/SuperAnimated.png")                    \
        \
        [C_DONODOPREDIO][CS_NORMAL]        = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/DonoDoPredio/Normal.png")                 \
        [C_DONODOPREDIO][CS_HAPPY]         = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/DonoDoPredio/Happy.png")                  \
        [C_DONODOPREDIO][CS_ANIMATED]      = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/DonoDoPredio/Animated.png")               \
        [C_DONODOPREDIO][CS_SAD]           = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/DonoDoPredio/Sad.png")                    \
        [C_DONODOPREDIO][CS_ANGRY]         = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/DonoDoPredio/Angry.png")                  \
        [C_DONODOPREDIO][CS_REVOLTED]      = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/DonoDoPredio/Revolted.png")               \
        [C_DONODOPREDIO][CS_DISTRUSTING]   = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/DonoDoPredio/Distrusting.png")            \
        [C_DONODOPREDIO][CS_DARK]          = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/DonoDoPredio/Dark.png")                   \
        [C_DONODOPREDIO][CS_SUPERANIMATED] = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/DonoDoPredio/SuperAnimated.png")          \
        \
        [C_INSPETOR][CS_NORMAL]        = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Inspetor/Normal.png")                         \
        [C_INSPETOR][CS_HAPPY]         = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Inspetor/Happy.png")                          \
        [C_INSPETOR][CS_ANIMATED]      = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Inspetor/Animated.png")                       \
        [C_INSPETOR][CS_SAD]           = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Inspetor/Sad.png")                            \
        [C_INSPETOR][CS_ANGRY]         = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Inspetor/Angry.png")                          \
        [C_INSPETOR][CS_REVOLTED]      = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Inspetor/Revolted.png")                       \
        [C_INSPETOR][CS_DISTRUSTING]   = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Inspetor/Distrusting.png")                    \
        [C_INSPETOR][CS_DARK]          = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Inspetor/Dark.png")                           \
        [C_INSPETOR][CS_SUPERANIMATED] = IMG_Load(CHARACTER_TEX_PATHS_PREFIX"/Inspetor/SuperAnimated.png")                  \
        \
    }


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
