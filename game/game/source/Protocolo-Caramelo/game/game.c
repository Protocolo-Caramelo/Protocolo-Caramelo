#include"game.h"
#include"screens/play/monitor.h"
#include"game_globals.h"

// Game methods
void Game_Init()
{
    graphical_init();
    game_globals_init();
    Monitor_Init();
}

void Game_Quit()
{
    Monitor_Quit();
    game_globals_quit();
    graphical_quit();
    SDL_Quit();
    exit(0);
}

// Play methods
void Game_PlayInit()
{
    game_state = GAME_STATE_SCREEN_PLAY;
    game_playState = GAME_PLAY_STATE_VIEW_ROOM_1;
    gamePlay_tex = GPLAY_TEX_ROOM_1;
}

void Game_PlayQuit()
{
    Game_Quit();
}


void Game_PlayGenerateProblem()
{
}

void Game_PlayManageMathProblem()
{
}


void Game_PlayDisplayBackground(GamePlay_Texture tex, SDL_Rect *tex_rect, SDL_Rect *tex_rectRaw, double*scale_width, double*scale_height)
{
    SDL_Texture *t;
    int tex_w, tex_h;
    int tex_nW, tex_nH;
    SDL_Rect rect, rectRaw;

    t = play_tex[tex];
    SDL_GetWindowSize(w, &w_width, &w_height);
    w_ratio = w_width / (double)w_height;
    SDL_QueryTexture(
        t,
        NULL, NULL,
        &tex_w, &tex_h
    );

    if(w_width / (double)w_height < tex_w / (double)tex_h){
        tex_nW = w_width;
        tex_nH = w_width * (GCORE_WINDOW_H / (double)GCORE_WINDOW_W);
    }
    else {
        tex_nH = w_height;
        tex_nW = w_height * (GCORE_WINDOW_W / (double)GCORE_WINDOW_H);
    }

    rect = (SDL_Rect){
        .x = w_width / 2 - tex_nW / 2,
        .y = w_height / 2 - tex_nH / 2,
        .w = tex_nW,
        .h = tex_nH
    };
    rectRaw = (SDL_Rect){
        .x = w_width / 2 - tex_nW / 2,
        .y = w_height / 2 - tex_nH / 2,
        .w = tex_w,
        .h = tex_h
    };

    //
    if(tex_rect){
        *tex_rect = rect;
    }
    if(tex_rectRaw){
        *tex_rectRaw = rectRaw;
    }

    if(scale_width){
        *scale_width = tex_rect->w / (double)rectRaw.w;
    }
    if(scale_height){
        *scale_height = tex_rect->h / (double)rectRaw.h;
    }

    SDL_RenderCopy(
        r,
        t,
        NULL,
        tex_rect
    );
}

void Game_PlayDisplayButtonArrow(Button*b, SDL_Rect*bg_texDest, int x, int y)
{
    double scale;
    //
    int tex_w, tex_h;
    //
    int b_borderL, b_borderR, b_borderB;
    int b_borderLPrev, b_borderRPrev, b_borderBPrev;
    double scale_width, scale_height;
    double scale_widthPrev, scale_heightPrev;
    char *str_prev;
    //
    size_t i;

    //
    b_borderLPrev = b_borderL       = b->state_tc[INP_STATE_NEUTRAL]->border.left;
    b_borderRPrev = b_borderR       = b->state_tc[INP_STATE_NEUTRAL]->border.right;
    b_borderBPrev = b_borderB       = b->state_tc[INP_STATE_NEUTRAL]->border.bottom;
    scale_widthPrev = scale_width   = b->state_tc[INP_STATE_NEUTRAL]->scale_width;
    scale_heightPrev = scale_height = b->state_tc[INP_STATE_NEUTRAL]->scale_height;
    str_prev                        = malloc(sizeof(Text_GetLenUTF8(b->text)));

    if(w_width / (double)w_height < bg_texDest->w / (double)bg_texDest->h){
        scale = w_width / (double)GCORE_WINDOW_W;
    }
    else {
        scale = w_height / (double)GCORE_WINDOW_H;
    }

    b_borderR    *= scale;
    b_borderL    *= scale;
    b_borderB    *= scale;
    scale_width  *= scale;
    scale_height *= scale;
    utf8cpy(str_prev, *Text_raw(b->text));

    for(i=0; i < INP_STATES_MAX; ++i){
        b->state_tc[i]->border.left = b_borderL;
        b->state_tc[i]->border.right = b_borderR;
        b->state_tc[i]->border.bottom = b_borderB;
        //
        b->state_tc[i]->scale_width = scale_width;
        b->state_tc[i]->scale_height = scale_height;
        //
    }
    if(!b->state[INP_STATE_SELECTED] && !b->state[INP_STATE_HOVER]){
        free(*Text_raw(b->text));
        *Text_raw(b->text) = NULL;
    }

    //
    //*
    Text_MeasureUTF8(b->text, NULL, NULL);
    Text_MeasureMargin(b->text, NULL, NULL);
    x -= ((*Text_edgesBack(b->text))[DRAW_EDGE_MARGIN_XMAX] - (*Text_edgesBack(b->text))[DRAW_EDGE_MARGIN_XMIN]) / 2;
    y -= ((*Text_edgesBack(b->text))[DRAW_EDGE_MARGIN_YMAX] - (*Text_edgesBack(b->text))[DRAW_EDGE_MARGIN_YMIN]) / 2;
    // */
    /*
    x -= (*Text_edges(b->text))[DRAW_WIDTH] / 2;
    y -= (*Text_edges(b->text))[DRAW_HEIGHT] / 2;
    // */
    Button_Display(
        b,
        x,
        y,
        NULL
    );

    for(i=0; i < INP_STATES_MAX; ++i){
        b->state_tc[i]->border.left = b_borderLPrev;
        b->state_tc[i]->border.right = b_borderRPrev;
        b->state_tc[i]->border.bottom = b_borderBPrev;
        //
        b->state_tc[i]->scale_width = scale_widthPrev;
        b->state_tc[i]->scale_height = scale_heightPrev;
    }

    //
    tex_w = b_arrowState[GBUTTON_ARROW_STATE_NEUTRAL].w;
    for(i=0; i < INP_STATES_MAX && (*Text_edges(b->text))[DRAW_WIDTH] < tex_w * GPLAY_BUTTON_ARROW_TEXTURE_SCALE * scale; ++i){
        b->state_tc[i]->border.right =
        b->state_tc[i]->border.left =
            ((tex_w + 2) * GPLAY_BUTTON_ARROW_TEXTURE_SCALE
            - (*Text_edges(b->text))[DRAW_WIDTH]) / 2.0
            ;
    }

    //
    Text_SetUTF8(b->text, str_prev);
    free(str_prev);
}

// Other usefull functions
double Game_GetScaleW()
{
    int width, height;
    SDL_GetWindowSize(w, &width, &height);
    return width / (double)GCORE_WINDOW_W;
}

double Game_GetScaleH()
{
    int width, height;
    SDL_GetWindowSize(w, &width, &height);
    return height / (double)GCORE_WINDOW_H;
}

void Game_ResetButtons()
{
    size_t i;
    Button *b;

    for(i=0; i < MENU_BUTTONS_MAX; ++i){
        b = menu_buttons[i];
        memset(b->state, 0, sizeof(b->state));
    }

    for(i=0; i < GPLAY_BUTTON_COUNT; ++i){
        b = play_buttons[i];
        memset(b->state, 0, sizeof(b->state));
    }
}

//
void Game_Debug()
{
#ifdef GAME_DEBUG
    SDL_GetWindowSize(
        w,
        &w_width, &w_height
    );

    if(SDL_GetTicks() - fps_delayTime >= fps_delay){
        sprintf(fps_buffer, "FPS: %.2lf", 1000.0 / (SDL_GetTicks() - fps_time));
        fps_delayTime = SDL_GetTicks();
    }
    Text_DrawUTF8(
        fps,
        fps_buffer,
        TextConfig_Create(
            .x = w_width - (*Text_edges(fps))[DRAW_WIDTH],
            .y = 0,
        ),
        NULL
    );

    fps_time = SDL_GetTicks();

#endif
}
