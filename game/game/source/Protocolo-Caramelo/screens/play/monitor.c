#include"monitor.h"
#include"monitor/monitor_desktop.h"
#include"screens/screens_communs.h"

static SDL_Rect m_texDest;
static SDL_Rect m_texDestRaw;
static double m_texScaleW, m_texScaleH;

//
// Life cycle
//
void Monitor_Init()
{
    MonitorDesktop_Init();
}

void Monitor_Quit()
{
    MonitorDesktop_Quit();
}

//
// Input
//
void Monitor_handleInput(SDL_Event*e)
{
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
        MonitorDesktop_Leave();
        game_playState  = GAME_PLAY_STATE_VIEW_ROOM_1;
        gamePlay_tex    = GPLAY_TEX_ROOM_1;
    }

    switch(e->type){
        case SDL_QUIT:
            Game_PlayQuit();
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

void Monitor_displayButtons()
{
    Button *b;
    int x, y;

    // Button back (power)
    b = play_buttons[GPLAY_BUTTON_MONITOR_BACK];
    x = m_texDest.x + 274 * m_texScaleW;
    y = m_texDest.y + 1324 * m_texScaleH;
    Monitor_displayButtonPower(b, x, y);
}

//
// Render
//
void Monitor_Render()
{
    SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
    SDL_RenderClear(r);

    while(SDL_PollEvent(&e)){
        Monitor_handleInput(&e);
    }

    Game_PlayDisplayBackground(
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
}
