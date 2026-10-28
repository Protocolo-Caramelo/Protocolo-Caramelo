#include"game_globals.h"

// SDL
SDL_Window *w;
SDL_Renderer *r;
SDL_Event e;
// Cache
TextCache *cache_fRegular;
TextCache *cache_fTitle;
TextCache *cache_f3D1;
// Game State
GameState gameState;
// Menu
TextConfig* menu_textConfig;
TextConfig* menu_buttonConfig[3];
// Play
SDL_Texture *room_1;
SDL_Texture *room_2;
SDL_Texture *monitor;

Text     *menu_tTitle[MENU_TITLE_COUNT];
Button   *menu_buttons[MENU_BUTTONS_MAX];
// Devquote(Debug)
#ifdef GAME_DEBUG
Text *fps;
int fps_time;
int fps_delayTime;
const int fps_delay = 100;
char fps_buffer[32];
#endif
// Mathematic
extern MathProblem problem;
// Others
int w_width;
int w_height;


void game_globals_init()
{
    // SDL
    extern SDL_Window *w;
    extern SDL_Renderer *r;
    extern SDL_Event e;
    // Cache
    extern TextCache *cache_fRegular;
    extern TextCache *cache_fTitle;
    extern TextCache *cache_f3D1;
    // Game State
    extern GameState gameState;
    // Menu
    extern TextConfig* menu_textConfig;
    extern TextConfig* menu_buttonConfig[3];

    extern Text     *menu_tTitle[2];
    extern Button   *menu_buttons[MENU_BUTTONS_MAX];

    // Play
    extern SDL_Texture *room_1;
    extern SDL_Texture *room_2;
    extern SDL_Texture *monitor;

    // Others
    extern int w_width;
    extern int w_height;
    //
    size_t i;

    w = SDL_CreateWindow(
        ELEMENT_WINDOW_NAME,
        ELEMENT_WINDOW_X_POSITION,
        ELEMENT_WINDOW_Y_POSITION,
        ELEMENT_WINDOW_W,
        ELEMENT_WINDOW_H,
        ELEMENT_WINDOW_FLAGS
    );
    r = SDL_CreateRenderer(
        w,
        -1,
        ELEMENT_RENDERER_FLAGS
    );
    //
    cache_fRegular = TextCache_Create(
        r,
        ELEMENT_CACHE_FREGULAR_PATH,
        ELEMENT_CACHE_FREGULAR_PTSIZE
    );
    cache_fTitle = TextCache_Create(
        r,
        ELEMENT_CACHE_FTITLE_PATH,
        ELEMENT_CACHE_FTITLE_PTSIZE
    );
    cache_f3D1 = TextCache_Create(
        r,
        ELEMENT_CACHE_3DFONT_1_PATH,
        ELEMENT_CACHE_3DFONT_1_PTSIZE
    );
    //
    gameState = GAME_STATE_SCREEN_MENU;
    //
    menu_textConfig = 
        TextConfig_Create(
            .scale_width = 1 / 2.0,
            .scale_height = 1 / 2.0,
            .color = (SDL_Color){255, 180, 27, 255}
        )
        ;
    menu_buttonConfig[INP_STATE_NEUTRAL]  = MENU_BUTTON_CONFIG_NEUTRAL;
    menu_buttonConfig[INP_STATE_SELECTED] = MENU_BUTTON_CONFIG_SELECTED;
    menu_buttonConfig[INP_STATE_HOVER]    = MENU_BUTTON_CONFIG_HOVER;

    menu_tTitle[MENU_TITLE_PROTOCOLO] = Text_Create(
        cache_f3D1,
        menu_textConfig,
        "Protocolo"
    );
    menu_tTitle[MENU_TITLE_CARAMELO] = Text_Create(
        cache_f3D1,
        menu_textConfig,
        "C.A.R.A.M.E.L.O"
    );
    *Text_scaleWidth(menu_tTitle[MENU_TITLE_CARAMELO]) *= 1.5;
    *Text_scaleHeight(menu_tTitle[MENU_TITLE_CARAMELO]) *= 1.5;

    menu_buttons[MENU_BUTTON_INIT]      = Button_Create(cache_fRegular, &menu_buttonConfig, "Novo jogo"); 
    menu_buttons[MENU_BUTTON_CONTINUE]  = Button_Create(cache_fRegular, &menu_buttonConfig, "Continuar");
    menu_buttons[MENU_BUTTON_CONFIG]    = Button_Create(cache_fRegular, &menu_buttonConfig, "Configurar");
    menu_buttons[MENU_BUTTON_EXIT]      = Button_Create(cache_fRegular, &menu_buttonConfig, "Sair");
    menu_buttons[MENU_BUTTON_ABOUT]     = Button_Create(cache_fRegular, &menu_buttonConfig, "Sobre");

    // Play
    room_1 = IMG_LoadTexture(r, SCREEN_ROOM_1_TEXTURE_PATH);
    room_2 = IMG_LoadTexture(r, SCREEN_ROOM_2_TEXTURE_PATH);
    monitor = IMG_LoadTexture(r, SCREEN_MONITOR_TEXTURE_PATH);

    // Devquote(Debug)
#ifdef GAME_DEBUG
    fps = Text_Create(
        cache_fRegular,
        FPS_CONFIG
    );
    fps_time = fps_delayTime = SDL_GetTicks();
#endif
}

void game_globals_quit()
{
    size_t i;
    // Menu
    for(i=0; i < MENU_BUTTONS_MAX; ++i){
        Button_Destroy(&menu_buttons[i]);
    }
    for(i=0; i < MENU_TITLE_COUNT; ++i){
        Text_Destroy(&menu_tTitle[i]);
    }

    // Cache
    TextCache_Destroy(&cache_fRegular);
    TextCache_Destroy(&cache_fTitle);
    TextCache_Destroy(&cache_f3D1);

    // Texture
    SDL_DestroyTexture(room_1);
    SDL_DestroyTexture(room_2);
    SDL_DestroyTexture(monitor);

    // SDL
    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(w);
}
