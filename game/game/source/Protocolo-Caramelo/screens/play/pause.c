#include"pause.h"
#include"screens/play.h"
#include"screens/menu.h"
#include"screens/screens_communs.h"

static SDL_Rect p_texDest;
static SDL_Rect p_texDestRaw;
static double p_texScaleW, p_texScaleH;
//
static Game_State       game_statePrev;
static Game_PlayState   game_playStatePrev;
static GamePlay_Texture gamePlay_texPrev;

static SDL_Texture *screen_prev;
static int text_protocoloEdges[DRAW_EDGE_AMT];
static int text_carameloEdges[DRAW_EDGE_AMT];
static int button_stateChanged;

void Pause_displayGameTitle()
{
    Text *t;

    t = play_text[GPLAY_TEXT_PAUSED_TITLE_PROTOCOLO];
    Text_DrawUTF8(
        t,
        NULL,
        TextConfig_Create(
            .x = w_width / 2 - text_protocoloEdges[DRAW_WIDTH] / 2,
            .y = 10,
        ),
        &text_protocoloEdges
    );

    t = play_text[GPLAY_TEXT_PAUSED_TITLE_CARAMELO];
    Text_DrawUTF8(
        t,
        NULL,
        TextConfig_Create(
            .x = text_protocoloEdges[DRAW_EDGE_XMAX] - text_protocoloEdges[DRAW_WIDTH] / 2 - text_carameloEdges[DRAW_WIDTH] / 2,
            .y = text_protocoloEdges[DRAW_EDGE_YMAX] + 5,
        ),
        &text_carameloEdges
    );
}

void Pause_displayButtons()
{
    Button *b;
    int x, y;
    int (*edges)[DRAW_EDGE_AMT];
    size_t i;

    edges = Text_edges(play_text[GPLAY_TEXT_PAUSED_TITLE_CARAMELO]);
    x = (*edges)[DRAW_EDGE_XMAX] - (*edges)[DRAW_WIDTH] / 2;
    y = (*edges)[DRAW_EDGE_YMAX] + 50;

    for(i=GPLAY_BUTTON_PAUSED_RESUME; i <= GPLAY_BUTTON_PAUSED_QUIT; ++i){
        b = play_buttons[i];
        x =
            (!button_stateChanged)?
            (*edges)[DRAW_EDGE_XMIN] + (*edges)[DRAW_WIDTH] / 2 - (*Text_edges(b->text))[DRAW_TEXT_WIDTH] / 2 
            : *Text_x(b->text)
            ;
        y =
            (!button_stateChanged)?
            y
            : *Text_y(b->text)
            ;

        Menu_displayButtonCentered(b, x, y, 0);

        edges = Text_edges(b->text);
        y = (*edges)[DRAW_EDGE_YMAX] + 20;
    }
}

void Pause_displayBackground()
{
    SDL_Texture *t;
    int tex_w, tex_h;

    t = play_tex[GPLAY_TEX_PAUSED];
    SDL_QueryTexture(
        t,
        NULL, NULL,
        &tex_w, &tex_h
    );
    //
    SDL_RenderCopy(
        r,
        screen_prev,
        NULL,
        &p_texDest
    );
    Play_DisplayBackground(
        gamePlay_tex,
        &p_texDest, &p_texDestRaw,
        &p_texScaleW, &p_texScaleH
    );
}


void Pause_handleKeyDown(SDL_Event*e)
{
    SDL_Scancode key;

    key = e->key.keysym.sym;
    if(key == SDLK_ESCAPE){
        Pause_QuitToGame();
    }
}

void Pause_handleInput(SDL_Event*e)
{
    size_t i;

    switch(e->type){
        case SDL_QUIT:
            Game_Quit();
            break;

        case SDL_KEYDOWN:
            Pause_handleKeyDown(e);
            break;

        default:
            break;
    }

    for(i=GPLAY_BUTTON_PAUSED_RESUME; i <= GPLAY_BUTTON_PAUSED_QUIT; ++i){
        Button_handleInput(play_buttons[i], e);
    }

    if(play_buttons[GPLAY_BUTTON_PAUSED_RESUME]->state[INP_STATE_SELECTED]){
        Pause_QuitToGame();
    }
    if(play_buttons[GPLAY_BUTTON_PAUSED_QUIT]->state[INP_STATE_SELECTED]){
        Pause_QuitToMenu();
    }
}

//
void Pause_Render()
{
    Pause_displayBackground();
    Pause_displayGameTitle();
    Pause_displayButtons();

    while(SDL_PollEvent(&e)){
        Pause_handleInput(&e);
    }
}

void Pause_Load(SDL_Rect *texDest)
{
    SDL_Texture *screen;
    SDL_Surface *screen_surf;
    int s_w, s_h;

    game_playStatePrev = game_playState;
    game_statePrev = game_state;
    gamePlay_texPrev = gamePlay_tex;

    game_playState = GAME_PLAY_STATE_PAUSED;
    gamePlay_tex   = GPLAY_TEX_PAUSED;
    button_stateChanged = 0;

    // Make a "screenshot" of window
    SDL_GetRendererOutputSize(r, &s_w, &s_h);
    screen_surf = SDL_CreateRGBSurfaceWithFormat(
        0,
        s_w, s_h,
        32, SDL_PIXELFORMAT_RGBA8888
    );
    SDL_RenderReadPixels(
        r,
        NULL,
        SDL_PIXELFORMAT_RGBA8888,
        screen_surf->pixels,
        screen_surf->pitch
    );
    screen = SDL_CreateTextureFromSurface(r, screen_surf);
    SDL_FreeSurface(screen_surf);
    //
    screen_prev = SDL_CreateTexture(
        r,
        SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
        texDest->w, texDest->h
    );
    //
    SDL_SetRenderTarget(r, screen_prev);
    SDL_RenderCopy(
        r,
        screen,
        texDest,
        NULL
    );
    SDL_SetRenderTarget(r, NULL);

    int w, h;
    SDL_QueryTexture(
        screen,
        NULL, NULL,
        &w, &h
    );
    SDL_DestroyTexture(screen);
}

void Pause_QuitToGame()
{
    if(screen_prev != NULL){
        SDL_DestroyTexture(screen_prev);
    }
    game_state      = game_statePrev;
    game_playState  = game_playStatePrev;
    gamePlay_tex    = gamePlay_texPrev;
}

void Pause_QuitToMenu()
{
    SDL_DestroyTexture(screen_prev);
    game_state      = GAME_STATE_SCREEN_MENU;
    GamePlay_Quit();
}
