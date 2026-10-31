#include"monitor.h"

static SDL_Rect m_texDest;
static SDL_Rect m_texDestRaw;
double m_texScaleW, m_texScaleH;

void Monitor_handleInput(SDL_Event*e)
{
    size_t i;
    for(i=GAMEPLAY_BUTTON_MONITOR_BACK; i <= GAMEPLAY_BUTTON_MONITOR_ICON_DELIVERY; ++i){
        Button_handleInput(play_buttons[i], e);
    }

    //*
    if(play_buttons[GAMEPLAY_BUTTON_MONITOR_BACK]->state[INP_STATE_HOVER]){
        gamePlay_tex = GAMEPLAY_TEX_MONITOR_POWER_BUTTON_SELECTED;
    }
    else {
        gamePlay_tex = GAMEPLAY_TEX_MONITOR_POWER_BUTTON_UNSELECTED;
    }

    if(play_buttons[GAMEPLAY_BUTTON_MONITOR_BACK]->state[INP_STATE_SELECTED]){
        gamePlay_state  = GAMEPLAY_STATE_VIEW_ROOM_1;
        gamePlay_tex    = GAMEPLAY_TEX_ROOM_1;
    }

    // */
    switch(e->type){
        case SDL_QUIT:
            Game_PlayQuit();
            break;

        default:
            break;
    }
}


void Monitor_displayButtonIcon(Button*b, int x, int y)
{
    size_t i;
    int w, h;
    //
    TextConfig tc_prev[INP_STATES_MAX];

    Text_MeasureUTF8(b->text, NULL, NULL);
    Text_MeasureMargin(b->text, NULL, NULL);
    w = ((*Text_edgesBack(b->text))[DRAW_EDGE_MARGIN_XMAX] - (*Text_edgesBack(b->text))[DRAW_EDGE_MARGIN_XMIN]);
    h = ((*Text_edgesBack(b->text))[DRAW_EDGE_MARGIN_YMAX] - (*Text_edgesBack(b->text))[DRAW_EDGE_MARGIN_YMIN]);

    //
    for(i=0; i < INP_STATES_MAX; ++i){
        if(!b->state_tc[i]){
            continue;
        }
        tc_prev[i] = *b->state_tc[i];

        //
        b->state_tc[i]->border.right -= w * m_texScaleW / 2;
        b->state_tc[i]->border.left  -= w * m_texScaleH / 2;

        b->state_tc[i]->border.top   *= m_texScaleH;
        b->state_tc[i]->border.right *= m_texScaleH;
        b->state_tc[i]->border.left  *= m_texScaleH;
        b->state_tc[i]->wrap         *= m_texScaleH;
        b->state_tc[i]->scale_width  *= m_texScaleH;
        b->state_tc[i]->scale_height *= m_texScaleH;
    }
    x -= w / 2;
    y -= h / 2;
    Button_Display(b, x, y, NULL);
    for(i=0; i < INP_STATES_MAX; ++i){
        if(!b->state_tc[i]){
            continue;
        }
        *b->state_tc[i] = tc_prev[i];
    }
}

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

void Monitor_displayButton()
{
    Button *b;
    int x, y;
    double scale;

    // Scale
    scale = Game_GetScaleH();

    // Button back
    b = play_buttons[GAMEPLAY_BUTTON_MONITOR_BACK];
    x = m_texDest.x + 274 * m_texScaleW;
    y = m_texDest.y + 1324 * m_texScaleH;
    Monitor_displayButtonPower(b, x, y);

    // Icon mine
    b = play_buttons[GAMEPLAY_BUTTON_MONITOR_ICON_MINE];
    x = m_texDest.x + 1864 * m_texScaleW;
    y = m_texDest.y + 346  * m_texScaleH;
    Monitor_displayButtonIcon(b, x, y);

    // Icon delivery
    b = play_buttons[GAMEPLAY_BUTTON_MONITOR_ICON_DELIVERY];
    x = m_texDest.x + 1864 * m_texScaleW;
    y = m_texDest.y + 570  * m_texScaleH;
    Monitor_displayButtonIcon(b, x, y);
}


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
    Monitor_displayButton();
}
