#include"bed.h"
#include"game/game.h"
#include"screens/play.h"

static SDL_Rect b_texDest;
static SDL_Rect b_texDestRaw;
static double b_texScaleW, b_texScaleH;
//
static int game_playStatePrev;
static int gamePlay_texPrev;
//
static int time_sleepMin;
static int time_sleepHour;
static char time_sleepBuffer[10];
//
static SDL_Texture *screen_prev;
static SDL_Rect b_texDest;

void Bed_displayBackground()
{
    SDL_RenderCopy(
        r,
        screen_prev,
        NULL,
        &b_texDest
    );
    Play_DisplayBackground(
        gamePlay_tex,
        &b_texDest, &b_texDestRaw,
        &b_texScaleW, &b_texScaleH
    );
}

void Bed_displayText()
{
    Text *t;
    int x, y;

    t = play_text[GPLAY_TEXT_BED_TIME_SLEEP];
    *Text_x(t) = b_texDest.x + b_texDest.w / 2 - (*Text_edges(t))[DRAW_WIDTH] / 2;
    *Text_y(t) = b_texDest.y + b_texDest.h / 2 - (*Text_edges(t))[DRAW_HEIGHT] / 2;
    sprintf(time_sleepBuffer, "%02i:%02i", time_sleepHour, time_sleepMin);
    Text_SetUTF8(t, time_sleepBuffer);
    Play_DisplayText(t, MIN(b_texScaleW, b_texScaleH));
}

void Bed_displayButtons()
{
    Button *b;
    Text *t;
    int x, y;

    t = play_text[GPLAY_TEXT_BED_TIME_SLEEP];
    //
    b = play_buttons[GPLAY_BUTTON_BED_INCR_HOUR];
    x = (*Text_edges(t))[DRAW_EDGE_XMIN] + 330 * b_texScaleW;
    y = (*Text_edges(t))[DRAW_EDGE_YMIN] - 30 * b_texScaleH;
    Play_DisplayButtonArrow(b, &b_texDest, x, y);

    //
    b = play_buttons[GPLAY_BUTTON_BED_INCR_MIN];
    x = (*Text_edges(t))[DRAW_EDGE_XMIN] + 1290 * b_texScaleW;
    y = (*Text_edges(t))[DRAW_EDGE_YMIN] - 30 * b_texScaleH;
    Play_DisplayButtonArrow(b, &b_texDest, x, y);

    //
    b = play_buttons[GPLAY_BUTTON_BED_DECR_HOUR];
    x = (*Text_edges(t))[DRAW_EDGE_XMIN] + 330 * b_texScaleW;
    y = (*Text_edges(t))[DRAW_EDGE_YMAX] + 30 * b_texScaleH;
    Play_DisplayButtonArrow(b, &b_texDest, x, y);

    //
    b = play_buttons[GPLAY_BUTTON_BED_DECR_MIN];
    x = (*Text_edges(t))[DRAW_EDGE_XMIN] + 1290 * b_texScaleW;
    y = (*Text_edges(t))[DRAW_EDGE_YMAX] + 30 * b_texScaleH;
    Play_DisplayButtonArrow(b, &b_texDest, x, y);

    //
    b = play_buttons[GPLAY_BUTTON_BED_SLEEP];
}

void Bed_handleKeyDown()
{
}

void Bed_handleInput(SDL_Event*e)
{
    Button *b;
    size_t i;

    switch(e->type){
        case SDL_QUIT:
            Game_Quit();
            break;

        case SDL_KEYDOWN:
            // Bed_handleKeyDown(e);
            break;
            
        default:
            break;
    }
    //
    for(i=GPLAY_BUTTON_BED_INCR_HOUR; i <= GPLAY_BUTTON_BED_BACK; ++i){
        b = play_buttons[i];
        Button_handleInput(b, e);
    }

    if(play_buttons[GPLAY_BUTTON_BED_INCR_HOUR]->state[INP_STATE_SELECTED]){
        ++time_sleepHour;
    }
    if(play_buttons[GPLAY_BUTTON_BED_INCR_MIN]->state[INP_STATE_SELECTED]){
        ++time_sleepMin;
    }
    if(play_buttons[GPLAY_BUTTON_BED_DECR_HOUR]->state[INP_STATE_SELECTED]){
        --time_sleepHour;
    }
    if(play_buttons[GPLAY_BUTTON_BED_DECR_MIN]->state[INP_STATE_SELECTED]){
        --time_sleepMin;
    }

    time_sleepHour %= 24;
    if(time_sleepMin >= 60){
        ++time_sleepHour;
        time_sleepMin %= 60;
    }
    //
    if(time_sleepMin < 0){
        --time_sleepHour;
        time_sleepMin = 59;
    }
    if(time_sleepHour < 0){
        time_sleepHour = 23;
    }
}


void Bed_Render()
{
    Bed_displayBackground();
    Bed_displayText();
    Bed_displayButtons();

    while(SDL_PollEvent(&e)){
        Bed_handleInput(&e);
    }
}

void Bed_Load(SDL_Rect *texDest)
{
    SDL_Texture *screen;

    game_playStatePrev = game_playState;
    gamePlay_texPrev   = gamePlay_tex;
    //
    game_playState     = GAME_PLAY_STATE_BED;
    gamePlay_tex       = GPLAY_TEX_PAUSED;

    screen_prev = SDL_CreateTexture(
        r,
        SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,
        texDest->w, texDest->h
    );
    b_texDest = *texDest;
    //
    screen = Play_TakeWindowScreenShot();
    SDL_SetRenderTarget(r, screen_prev);
    SDL_RenderCopy(
        r,
        screen,
        texDest,
        NULL
    );
    SDL_SetRenderTarget(r, NULL);
    SDL_DestroyTexture(screen);
}

void Bed_Leave()
{
    SDL_DestroyTexture(screen_prev);
    game_playState = game_playStatePrev;
    gamePlay_tex   = gamePlay_texPrev;
}
