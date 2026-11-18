#include"monitor.h"
#include"monitor/monitor_desktop.h"
#include"screens/play.h"
#include"screens/play/room.h"
#include"screens/play/pause.h"
#include"screens/screens_communs.h"
#include"mechanics/mechanics.h"

static SDL_Rect m_texDest;
static SDL_Rect m_texDestRaw;
static double m_texScaleW, m_texScaleH;
//
static int leaved;
//
static char clock_buffer[250];
static int clock_timeEdges[DRAW_EDGE_AMT];
static int clock_dateEdges[DRAW_EDGE_AMT];

//
void Monitor_handleKeyDown(SDL_Event *e)
{
    SDL_Scancode key;

    key = e->key.keysym.sym;
    switch(key){
        case SDLK_ESCAPE:
            if(in_transition){
                break;
            }
            Monitor_Leave();
            Pause_Load(&m_texDest);
            break;

#ifdef GAME_DEBUG
        // Dev shortcuts: play the demo scenes while the monitor is open, to see
        // them show up in the Bate-boca app (see mechanics/Dialog/dialog_scripts.c)
        case SDLK_F2:
            DialogScripts_FelipeCall();
            break;
        case SDLK_F3:
            // DialogScripts_AnaCall();
            break;
#endif

        default:
            break;
    }
}

void Monitor_handleInput(SDL_Event*e)
{
    if(leaved){
        return;
    }

    // An active dialog owns the mouse and the confirm keys
    if(Dialog_HandleInput(e)){
        return;
    }

    // Windows and icons live "inside" the monitor. The power button is outside of
    // the desktop area, so it always receives the event too (keeps its hover state right)
    MonitorDesktop_HandleEvent(e);

    Button_handleInput(play_buttons[GPLAY_BUTTON_MONITOR_BACK], e);
    if(play_buttons[GPLAY_BUTTON_MONITOR_BACK]->state[INP_STATE_HOVER]){
        gamePlay_tex = GPLAY_TEX_MONITOR_POWER_BUTTON_SELECTED;
    }
    else {
        gamePlay_tex = GPLAY_TEX_MONITOR_POWER_BUTTON_UNSELECTED;
    }
    if(play_buttons[GPLAY_BUTTON_MONITOR_BACK]->state[INP_STATE_SELECTED]){
        Monitor_Leave();
        Room1_Load();
    }

    switch(e->type){
        case SDL_QUIT:
            Game_Quit();
            break;

        case SDL_KEYDOWN:
            Monitor_handleKeyDown(e);
            break;

        default:
            break;
    }
}

//
// Buttons
//
void Monitor_displayButtonPower(Button*b, int x, int y)
{
    size_t i;
    int bx_width, bx_height;

    bx_width = b->state_tc[INP_STATE_NEUTRAL]->bx_width;
    bx_height = b->state_tc[INP_STATE_NEUTRAL]->bx_height;
    //
    for(i=0; i < INP_STATES_MAX; ++i){
        if(!b->state_tc[i]){
            continue;
        }
        b->state_tc[i]->bx_width  *= m_texScaleH;
        b->state_tc[i]->bx_height *= m_texScaleH;
    }
    Button_Display(b, x, y, NULL);
    for(i=0; i < INP_STATES_MAX; ++i){
        if(!b->state_tc[i]){
            continue;
        }
        b->state_tc[i]->bx_width  = bx_width;
        b->state_tc[i]->bx_height = bx_height;
    }
}

void Monitor_displayStatus()
{
    Play_DisplayStatus(&m_texDest, m_texScaleW, m_texScaleH);
}

void Monitor_displayButtons()
{
    Button *b;
    int x, y;

    // Button back (power)
    b = play_buttons[GPLAY_BUTTON_MONITOR_BACK];
    x = m_texDest.x + 282 * m_texScaleW;
    y = m_texDest.y + 1237 * m_texScaleH;
    Monitor_displayButtonPower(b, x, y);
}

void Monitor_displayClock()
{
    Text *t;
    TextConfig tc;

    t = play_text[GPLAY_TEXT_MONITOR_CLOCK_TIME];
    tc = t->config;
    Play_GetClockFormat(clock_buffer, MONITOR_CLOCK_FMT_TIME);
    Text_DrawUTF8(
        t,
        clock_buffer,
        TextConfig_Create(
            .scale_width  = tc.scale_width * m_texScaleW,
            .scale_height = tc.scale_height * m_texScaleW,
            .x = m_texDest.x + 1700 * m_texScaleW,
            .y = m_texDest.y + 1230 * m_texScaleH,
        ),
        &clock_timeEdges
    );
    t->config = tc;

    t = play_text[GPLAY_TEXT_MONITOR_CLOCK_DATE];
    tc = t->config;
    Play_GetClockFormat(clock_buffer, MONITOR_CLOCK_FMT_DATE);
    Text_DrawUTF8(
        t,
        clock_buffer,
        TextConfig_Create(
            .scale_width  = tc.scale_width * m_texScaleW,
            .scale_height = tc.scale_height * m_texScaleW,
            .x = clock_timeEdges[DRAW_EDGE_XMAX] - clock_timeEdges[DRAW_WIDTH] / 2 - clock_dateEdges[DRAW_WIDTH] / 2,
            .y = clock_timeEdges[DRAW_EDGE_YMAX] + 10 * m_texScaleH,
        ),
        &clock_dateEdges
    );
    t->config = tc;
}

//
// Render
//
void Monitor_Render()
{
    //
    SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
    SDL_RenderClear(r);

    Play_DisplayBackground(
        gamePlay_tex,
        &m_texDest, &m_texDestRaw,
        &m_texScaleW, &m_texScaleH
    );

    // The desktop is drawn over the background, under the power button. If the
    // player just left the monitor, this frame still shows the room instead
    MonitorDesktop_SetSpace(&m_texDest, m_texScaleW, m_texScaleH);
    if(game_playState == GAME_PLAY_STATE_VIEW_MONITOR){
        MonitorDesktop_Render();
    }
    Monitor_displayButtons();
    Monitor_displayClock();
    Monitor_displayStatus();

    while(SDL_PollEvent(&e)){
        Monitor_handleInput(&e);
    }
    leaved = 0;
}

void Monitor_Load()
{
    Play_TransitionInit(
        500,
        game_state,
        GAME_PLAY_STATE_VIEW_MONITOR,
        GPLAY_TEX_MONITOR_POWER_BUTTON_UNSELECTED
    );

    MonitorDesktop_Init();
    leaved = 0;
}

void Monitor_Leave()
{
    MonitorDesktop_Leave();
    leaved = 1;
}

void Monitor_Quit()
{
    MonitorDesktop_Quit();
}
