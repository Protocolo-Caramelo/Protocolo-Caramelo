#include"game_globals.h"

//
// SDL
//
SDL_Window *w;
SDL_Renderer *r;
SDL_Event e;
//
// Cache
//
TextCache *cache_fRegular;
TextCache *cache_fTitle;
TextCache *cache_f3D1;
//
// Buttons type
//
SDL_Rect b_arrowState[BUTTON_ARROW_STATE_COUNT];
SDL_Texture *b_arrowTex;
//
// Game State
//
GameState gameState;
GamePlay_State gamePlay_state;
//
// Menu
//
TextConfig *menu_textConfig;
TextConfig *menu_buttonConfig[INP_STATES_MAX];
Text        *menu_tTitle[MENU_TITLE_COUNT];
Button      *menu_buttons[MENU_BUTTONS_MAX];
//
// Play
//
GamePlay_Texture gamePlay_tex;
SDL_Texture *play_tex[GAMEPLAY_TEX_COUNT];
TextConfig  *play_buttonsConfig[INP_STATES_MAX];
Button      *play_buttons[GAMEPLAY_BUTTON_COUNT];
// 
// Devquote(Debug)
//
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
double w_ratio;


void game_globals_init()
{
    // SDL
    extern SDL_Window *w;
    extern SDL_Renderer *r;
    extern SDL_Event e;
    // Button
    extern SDL_Rect b_arrowState[BUTTON_ARROW_STATE_COUNT];
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
    extern SDL_Texture *play_tex[GAMEPLAY_TEX_COUNT];
    extern Button *play_buttons[GAMEPLAY_BUTTON_COUNT];

    // Others
    extern int w_width;
    extern int w_height;
    //
    size_t i;

    //
    // SDL2
    //
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
    // Buttons type
    //
    b_arrowTex = IMG_LoadTexture(r, BUTTON_ARROW_TEXTURE_PATH);
    if(!b_arrowTex){
        printf("Error while loading button arrow texture: %s\n", SDL_GetError());
    }
    b_arrowState[BUTTON_ARROW_STATE_NEUTRAL] = (SDL_Rect){
        .x = 108,
        .y = 64,
        .w = 94,
        .h = 135
    };
    b_arrowState[BUTTON_ARROW_STATE_SELECTED] = (SDL_Rect){
        .x = 269,
        .y = 64,
        .w = 94,
        .h = 135
    };
    b_arrowState[BUTTON_ARROW_STATE_HOVER] = b_arrowState[BUTTON_ARROW_STATE_SELECTED];
    //
    // Cache
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
    // Game
    //
    gameState = GAME_STATE_SCREEN_MENU;
    //
    // Menu
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

    //
    // Play
    //
    play_tex[GAMEPLAY_TEX_ROOM_1] = IMG_LoadTexture(r, GAMEPLAY_TEX_ROOM_1_PATH);
    play_tex[GAMEPLAY_TEX_ROOM_2] = IMG_LoadTexture(r, GAMEPLAY_TEX_ROOM_2_PATH);
    play_tex[GAMEPLAY_TEX_MONITOR] = IMG_LoadTexture(r, GAMEPLAY_TEX_MONITOR_PATH);
    for(i=0;  i < GAMEPLAY_TEX_COUNT; ++i){
        if(!play_tex[i]){
            printf("Error while open play textures: %s\n", SDL_GetError());
        }
    }

    play_buttonsConfig[INP_STATE_NEUTRAL]   = GAMEPLAY_BUTTON_ARROW_CONFIG_NEUTRAL;
    play_buttonsConfig[INP_STATE_SELECTED]  = GAMEPLAY_BUTTON_ARROW_CONFIG_SELECTED;
    play_buttonsConfig[INP_STATE_HOVER]     = GAMEPLAY_BUTTON_ARROW_CONFIG_HOVER;
    //
    play_buttons[GAMEPLAY_BUTTON_ARROW_ROOM1_BACK]      = Button_Create(cache_fRegular, &play_buttonsConfig, "Virar");
    play_buttons[GAMEPLAY_BUTTON_ARROW_ROOM1_MONITOR]   = Button_Create(cache_fRegular, &play_buttonsConfig, "Monitor");
    play_buttons[GAMEPLAY_BUTTON_ARROW_ROOM1_BED]       = Button_Create(cache_fRegular, &play_buttonsConfig, "Cama");

    play_buttons[GAMEPLAY_BUTTON_ARROW_ROOM2_BACK] = Button_Create(cache_fRegular, &play_buttonsConfig, "Virar");
    play_buttons[GAMEPLAY_BUTTON_ARROW_ROOM2_DOOR] = Button_Create(cache_fRegular, &play_buttonsConfig, "Porta");

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

    // Button
    SDL_DestroyTexture(b_arrowTex);

    // Menu
    for(i=0; i < MENU_BUTTONS_MAX; ++i){
        Button_Destroy(&menu_buttons[i]);
    }
    for(i=0; i < MENU_TITLE_COUNT; ++i){
        Text_Destroy(&menu_tTitle[i]);
    }

    // GamePlay
    for(i=0; i < GAMEPLAY_TEX_COUNT; ++i){
        SDL_DestroyTexture(play_tex[i]);
    }

    // Cache
    TextCache_Destroy(&cache_fRegular);
    TextCache_Destroy(&cache_fTitle);
    TextCache_Destroy(&cache_f3D1);

    // SDL
    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(w);
}

// Button type alternate functions
void gamePlay_Button_Arrow_Neutral_alterBorder(Text*t, Texture*tex)
{
    SDL_Rect dest;

    SDL_GetWindowSize(w, &w_width, &w_height);
    //
    dest = b_arrowState[BUTTON_ARROW_STATE_NEUTRAL];
    if(w_width / (double)w_height < dest.w / (double)dest.h){
        dest.w *= GAMEPLAY_BUTTON_ARROW_TEXTURE_SCALE * w_width / (double)ELEMENT_WINDOW_W;
        dest.h *= GAMEPLAY_BUTTON_ARROW_TEXTURE_SCALE * w_width / (double)ELEMENT_WINDOW_W;
    }
    else {
        dest.w *= GAMEPLAY_BUTTON_ARROW_TEXTURE_SCALE * w_height / (double)ELEMENT_WINDOW_W;
        dest.h *= GAMEPLAY_BUTTON_ARROW_TEXTURE_SCALE * w_height / (double)ELEMENT_WINDOW_W;
    }
    //
    dest.x = ((*Text_edgesBack(t))[DRAW_TEXT_WIDTH] + *Text_borderLeft(t) + *Text_borderRight(t)) / 2 - dest.w / 2;
    dest.y = (*Text_edgesBack(t))[DRAW_EDGE_TEXT_YMAX];
    //
    SDL_RenderCopy(
        r,
        b_arrowTex,
        &b_arrowState[BUTTON_ARROW_STATE_NEUTRAL],
        &dest
    );
}

void gamePlay_Button_Arrow_Selected_alterBorder(Text*t, Texture*tex)
{
    SDL_Rect dest;

    SDL_GetWindowSize(w, &w_width, &w_height);
    //
    dest = b_arrowState[BUTTON_ARROW_STATE_NEUTRAL];
    if(w_width / (double)w_height < dest.w / (double)dest.h){
        dest.w *= GAMEPLAY_BUTTON_ARROW_TEXTURE_SCALE * w_width / (double)ELEMENT_WINDOW_W;
        dest.h *= GAMEPLAY_BUTTON_ARROW_TEXTURE_SCALE * w_width / (double)ELEMENT_WINDOW_W;
    }
    else {
        dest.w *= GAMEPLAY_BUTTON_ARROW_TEXTURE_SCALE * w_height / (double)ELEMENT_WINDOW_W;
        dest.h *= GAMEPLAY_BUTTON_ARROW_TEXTURE_SCALE * w_height / (double)ELEMENT_WINDOW_W;
    }
    //
    dest.x = ((*Text_edgesBack(t))[DRAW_TEXT_WIDTH] + *Text_borderLeft(t) + *Text_borderRight(t)) / 2 - dest.w / 2;
    dest.y = (*Text_edgesBack(t))[DRAW_EDGE_TEXT_YMAX];
    //
    SDL_RenderCopy(
        r,
        b_arrowTex,
        &b_arrowState[BUTTON_ARROW_STATE_SELECTED],
        &dest
    );
}

void gamePlay_Button_Arrow_Hover_alterBorder(Text*t, Texture*tex)
{
    SDL_Rect dest;
    int w_width, w_height;

    SDL_GetWindowSize(w, &w_width, &w_height);
    //
    dest = b_arrowState[BUTTON_ARROW_STATE_NEUTRAL];
    if(w_width / (double)w_height < dest.w / (double)dest.h){
        dest.w *= GAMEPLAY_BUTTON_ARROW_TEXTURE_SCALE * w_width / (double)ELEMENT_WINDOW_W;
        dest.h *= GAMEPLAY_BUTTON_ARROW_TEXTURE_SCALE * w_width / (double)ELEMENT_WINDOW_W;
    }
    else {
        dest.w *= GAMEPLAY_BUTTON_ARROW_TEXTURE_SCALE * w_height / (double)ELEMENT_WINDOW_W;
        dest.h *= GAMEPLAY_BUTTON_ARROW_TEXTURE_SCALE * w_height / (double)ELEMENT_WINDOW_W;
    }
    //
    dest.x = ((*Text_edgesBack(t))[DRAW_TEXT_WIDTH] + *Text_borderLeft(t) + *Text_borderRight(t)) / 2 - dest.w / 2;
    dest.y = (*Text_edgesBack(t))[DRAW_EDGE_TEXT_YMAX];
    //
    SDL_RenderCopy(
        r,
        b_arrowTex,
        &b_arrowState[BUTTON_ARROW_STATE_HOVER],
        &dest
    );
}
