#include"bed.h"
#include"game/game.h"
#include"screens/play.h"
#include"screens/play/room.h"
#include<math.h>

static SDL_Rect b_texDest;
static SDL_Rect b_texDestRaw;
static double b_texScaleW, b_texScaleH;
//
static int game_playStatePrev;
static int gamePlay_texPrev;
//
static int slepping;
static int time_sleepMin;
static int time_sleepHour;
static char time_sleepBuffer[10];
static char bed_statusBuffer[96];
//
// Sleep session (game seconds)
static double sleep_remainSec;
static double sleep_elapsedSec;
static double sleep_msCarry;
static Uint32 sleep_lastTicks;
static double sleep_e0, sleep_t0, sleep_h0;   // status when the sleep started
static int    sleepBtn_prev;
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

static double Bed_sleepSecondsSelected()
{
    return (slepping)? sleep_remainSec: (time_sleepHour * 3600.0 + time_sleepMin * 60.0);
}

void Bed_displayButtons()
{
    Button *b;
    Text *t;
    int x, y;

    t = play_text[GPLAY_TEXT_BED_TIME_SLEEP];
    //
    b = play_buttons[GPLAY_BUTTON_BED_INCR_HOUR];
    if(slepping){
        b->state[INP_STATE_HOVER] = 1;
    }
    x = (*Text_edges(t))[DRAW_EDGE_XMIN] + 330 * b_texScaleW;
    y = (*Text_edges(t))[DRAW_EDGE_YMIN] - 30 * b_texScaleH;
    Play_DisplayButtonArrow(b, &b_texDest, x, y);

    //
    b = play_buttons[GPLAY_BUTTON_BED_INCR_MIN];
    if(slepping){
        b->state[INP_STATE_HOVER] = 1;
    }
    x = (*Text_edges(t))[DRAW_EDGE_XMIN] + 1290 * b_texScaleW;
    y = (*Text_edges(t))[DRAW_EDGE_YMIN] - 30 * b_texScaleH;
    Play_DisplayButtonArrow(b, &b_texDest, x, y);

    //
    b = play_buttons[GPLAY_BUTTON_BED_DECR_HOUR];
    if(slepping){
        b->state[INP_STATE_HOVER] = 1;
    }
    x = (*Text_edges(t))[DRAW_EDGE_XMIN] + 330 * b_texScaleW;
    y = (*Text_edges(t))[DRAW_EDGE_YMAX] + 30 * b_texScaleH;
    Play_DisplayButtonArrow(b, &b_texDest, x, y);

    //
    b = play_buttons[GPLAY_BUTTON_BED_DECR_MIN];
    if(slepping){
        b->state[INP_STATE_HOVER] = 1;
    }
    x = (*Text_edges(t))[DRAW_EDGE_XMIN] + 1290 * b_texScaleW;
    y = (*Text_edges(t))[DRAW_EDGE_YMAX] + 30 * b_texScaleH;
    Play_DisplayButtonArrow(b, &b_texDest, x, y);

    //
    b = play_buttons[GPLAY_BUTTON_BED_SLEEP];
    Text_SetUTF8(b->text, (slepping)? "Acordar": "Descansar");
    x = (*Text_edges(t))[DRAW_EDGE_XMIN] + (*Text_edges(t))[DRAW_WIDTH] / 2 - (*Text_edges(b->text))[DRAW_WIDTH] / 2;
    y = (*Text_edges(t))[DRAW_EDGE_YMAX] + 10 * b_texScaleH;
    Button_Display(b, x, y, NULL);

    //
    b = play_buttons[GPLAY_BUTTON_BED_BACK];
    x =
        (*Text_edges(t))[DRAW_EDGE_XMIN] + (*Text_edges(t))[DRAW_WIDTH] / 2 - (*Text_edges(b->text))[DRAW_WIDTH] / 2;
    y = (*Text_edges(t))[DRAW_EDGE_YMIN] - 30 * b_texScaleH;
    Button_Display(b, x, y, NULL);
}

void Bed_handleKeyDown(SDL_Event*e)
{
    SDL_Scancode key;

    key = e->key.keysym.sym;
    if(key == SDLK_ESCAPE){
        Bed_Leave();
    }
}

static void Bed_toggleSleep()
{
    if(slepping){
        slepping = 0;                      // wake up: keeps what was recovered
        return;
    }
    if(!time_sleepHour && !time_sleepMin){
        return;
    }
    sleep_remainSec  = time_sleepHour * 3600.0 + time_sleepMin * 60.0;
    sleep_elapsedSec = 0;
    sleep_msCarry    = 0;
    sleep_lastTicks  = SDL_GetTicks();
    sleep_e0 = i_energy;
    sleep_t0 = i_tired;
    sleep_h0 = i_health;
    slepping = 1;
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
            Bed_handleKeyDown(e);
            break;
            
        default:
            break;
    }
    //
    for(i=GPLAY_BUTTON_BED_INCR_HOUR; i <= GPLAY_BUTTON_BED_BACK; ++i){
        b = play_buttons[i];
        Button_handleInput(b, e);
    }

    if(play_buttons[GPLAY_BUTTON_BED_INCR_HOUR]->state[INP_STATE_SELECTED] && !slepping){
        ++time_sleepHour;
    }
    if(play_buttons[GPLAY_BUTTON_BED_INCR_MIN]->state[INP_STATE_SELECTED] && !slepping){
        ++time_sleepMin;
    }
    if(play_buttons[GPLAY_BUTTON_BED_DECR_HOUR]->state[INP_STATE_SELECTED] && !slepping){
        --time_sleepHour;
    }
    if(play_buttons[GPLAY_BUTTON_BED_DECR_MIN]->state[INP_STATE_SELECTED] && !slepping){
        --time_sleepMin;
    }
    //
    if(play_buttons[GPLAY_BUTTON_BED_BACK]->state[INP_STATE_SELECTED]){
        Bed_Leave();
    }

    if(play_buttons[GPLAY_BUTTON_BED_SLEEP]->state[INP_STATE_SELECTED]){
        if(!sleepBtn_prev){
            Bed_toggleSleep();
        }
        sleepBtn_prev = 1;
    }
    else {
        sleepBtn_prev = 0;
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

/*
 * Passa o tempo de sono. O cronômetro cai rápido (GINFO_SLEEP_ANIM_SEC_PER_HOUR
 * segundos reais por hora de jogo); a cada quadro o relógio do jogo, as contas
 * e os status avançam juntos. Os status seguem as curvas exponenciais de
 * GameInfos_SleepApply, calculadas a partir de quando o sono começou.
 */
void Bed_sleep()
{
    Uint32 now;
    double adv, dt;
    int total_min;

    if(!slepping){
        sleep_lastTicks = SDL_GetTicks();
        return;
    }

    now = SDL_GetTicks();
    dt  = (now - sleep_lastTicks) / 1000.0;
    sleep_lastTicks = now;
    if(dt > 0.1){
        dt = 0.1;                          // avoids a jump after a freeze
    }

    adv = MIN(dt * 13000.0 / GINFO_SLEEP_ANIM_SEC_PER_HOUR, sleep_remainSec);
    sleep_remainSec  -= adv;
    sleep_elapsedSec += adv;

    sleep_msCarry += adv * 1000.0;
    Clock_SumMs(gclock, (ABS_T_MS)sleep_msCarry);
    sleep_msCarry -= floor(sleep_msCarry);
    Bills_Advance(adv);

    GameInfos_SleepApply(sleep_elapsedSec / 3600.0, sleep_e0, sleep_t0, sleep_h0);

    total_min      = (int)ceil(sleep_remainSec / 60.0);
    time_sleepHour = total_min / 60;
    time_sleepMin  = total_min % 60;
    if(sleep_remainSec <= 0){
        slepping = 0;
        time_sleepHour = time_sleepMin = 0;
    }
}


void Bed_Render()
{
    Bed_sleep();
    Bed_displayBackground();
    Bed_displayText();
    Bed_displayButtons();
    //
    Play_DisplayStatus(&b_texDest, b_texScaleW, b_texScaleH);

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
    //
    slepping = 0;
}

void Bed_Leave()
{
    slepping = 0;
    if(screen_prev){
        SDL_DestroyTexture(screen_prev);
        screen_prev = NULL;
    }
    game_playState = game_playStatePrev;
    gamePlay_tex   = gamePlay_texPrev;
}
