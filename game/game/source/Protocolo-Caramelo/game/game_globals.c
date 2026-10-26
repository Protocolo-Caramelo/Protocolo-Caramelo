#include"game_globals.h"

// SDL
SDL_Window *w;
SDL_Renderer *r;
SDL_Event e;
// Cache
TextCache *cache_fRegular;
TextCache *cache_fTitle;
// Game State
GameState gameState;
// Menu
TextConfig* menu_textConfig;
TextConfig* menu_buttonConfig[3];

Text     *menu_tTitle;
Button   *menu_buttons[MENU_BUTTONS_MAX];
// Devquote(Debug)
#ifdef GAME_DEBUG
Text *fps;
int fps_time;
int fps_delayTime;
const int fps_delay = 100;
char fps_buffer[32];
#endif
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
    // Game State
    extern GameState gameState;
    // Menu
    extern TextConfig* menu_textConfig;
    extern TextConfig* menu_buttonConfig[3];

    extern Text     *menu_tTitle;
    extern Button   *menu_buttons[MENU_BUTTONS_MAX];
    //
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
    //
    gameState = GAME_STATE_SCREEN_MENU;
    //
    menu_textConfig = 
        TextConfig_Create(
            .scale_width = 1 / 2.0,
            .scale_height = 1 / 2.0,
            .color = (SDL_Color){.r=251, .g=223, .b=71, .a=255}
        )
        ;
    menu_buttonConfig[INP_STATE_NEUTRAL]  = MENU_BUTTON_CONFIG_NEUTRAL;
    menu_buttonConfig[INP_STATE_SELECTED] = MENU_BUTTON_CONFIG_SELECTED;
    menu_buttonConfig[INP_STATE_HOVER]    = MENU_BUTTON_CONFIG_HOVER;

    menu_tTitle = Text_Create(
        cache_fTitle,
        menu_textConfig,
        "Procolo-Caramelo"
    );
    menu_buttons[MENU_BUTTON_INIT]      = Button_Create(cache_fRegular, &menu_buttonConfig, "Começar"); 
    menu_buttons[MENU_BUTTON_CONTINUE]  = Button_Create(cache_fRegular, &menu_buttonConfig, "Continuar");
    menu_buttons[MENU_BUTTON_CONFIG]    = Button_Create(cache_fRegular, &menu_buttonConfig, "Configurar");
    menu_buttons[MENU_BUTTON_EXIT]      = Button_Create(cache_fRegular, &menu_buttonConfig, "Sair");
    menu_buttons[MENU_BUTTON_ABOUT]     = Button_Create(cache_fRegular, &menu_buttonConfig, "Sobre");

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
    Text_Destroy(&menu_tTitle);

    // Cache
    TextCache_Destroy(&cache_fRegular);
    TextCache_Destroy(&cache_fTitle);

    // SDL
    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(w);
}
