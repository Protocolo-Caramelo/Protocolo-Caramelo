#include"room.h"
#include"screens/play.h"
#include"screens/play/pause.h"
#include"screens/play/monitor.h"
#include"screens/play/bed.h"
#include"screens/screens_communs.h"
#include"mechanics/mechanics.h"

static SDL_Rect room_texDest;
static SDL_Rect room_texDestRaw;
static double room_texScaleW;
static double room_texScaleH;
//
static char clock_buffer[250];
static int clock_dateEdges[DRAW_EDGE_AMT];
static int clock_timeEdges[DRAW_EDGE_AMT];

void Room_displayButtonArrow(Button*b, int x, int y)
{
    Play_DisplayButtonArrow(b, &room_texDestRaw, x, y);
}

void Room_displayStatus()
{
    Play_DisplayStatus(&room_texDest, room_texScaleW, room_texScaleH);
}

void Room_displayBackground()
{
    Play_DisplayBackground(
        gamePlay_tex,
        &room_texDest, &room_texDestRaw,
        &room_texScaleW, &room_texScaleH 
    );
}

void Room_displayClock()
{
    Text *t;
    double scale_width, scale_height;
    size_t i;
    Padding p;

    //
    t = play_text[GPLAY_TEXT_ROOM_CLOCK_DATE];
    scale_width  = *Text_scaleWidth(t);
    scale_height = *Text_scaleHeight(t);
    p = *Text_padding(t);

    Play_GetClockFormat(clock_buffer, ROOM_CLOCK_FMT_DATE);
    Text_DrawUTF8(
        t,
        clock_buffer,
        TextConfig_Create(
            .scale_width  = scale_width * room_texScaleW,
            .scale_height = scale_height * room_texScaleW,
            .padding = (Padding){
                .top    = p.top    * room_texScaleW,
                .bottom = p.bottom * room_texScaleW,
                .right  = p.right  * room_texScaleW,
                .left   = p.left   * room_texScaleW,
            },
            .x = room_texDest.x + room_texDest.w - clock_timeEdges[DRAW_WIDTH] / 2 - clock_dateEdges[DRAW_WIDTH] / 2,
            // .x = room_texDest.x + room_texDest.w - clock_dateEdges[DRAW_WIDTH],
            .y = room_texDest.y,
            // .y = clock_timeEdges[DRAW_EDGE_YMAX],
        ),
        &clock_dateEdges
    );
    *Text_scaleWidth(t)  = scale_width;
    *Text_scaleHeight(t) = scale_height;
    *Text_padding(t) = p;

    //
    t = play_text[GPLAY_TEXT_ROOM_CLOCK_TIME];
    scale_width  = *Text_scaleWidth(t);
    scale_height = *Text_scaleHeight(t);
    p = *Text_padding(t);
    if(clock_timeEdges[DRAW_WIDTH] != clock_dateEdges[DRAW_WIDTH]){
        *Text_scaleWidth(t) *= clock_dateEdges[DRAW_WIDTH] / (double)clock_timeEdges[DRAW_WIDTH];
        *Text_scaleHeight(t) *= clock_dateEdges[DRAW_WIDTH] / (double)clock_timeEdges[DRAW_WIDTH];
    }

    Play_GetClockFormat(clock_buffer, ROOM_CLOCK_FMT_TIME);
    Text_DrawUTF8(
        t,
        clock_buffer,
        TextConfig_Create(
            .scale_width  = scale_width * room_texScaleW,
            .scale_height = scale_height * room_texScaleW,
            .padding = (Padding){
                .top    = p.top    * room_texScaleW,
                .bottom = p.bottom * room_texScaleW,
                .right  = p.right  * room_texScaleW,
                .left   = p.left   * room_texScaleW,
            },
            .x = room_texDest.x + room_texDest.w - clock_timeEdges[DRAW_WIDTH],
            .y = clock_dateEdges[DRAW_EDGE_YMAX],
            // .y = room_texDest.y,
        ),
        &clock_timeEdges
    );
    *Text_scaleWidth(t)  = scale_width;
    *Text_scaleHeight(t) = scale_height;
    *Text_padding(t) = p;
}


void Room_handleKeyDown(SDL_Event*e)
{
    SDL_Scancode key;

    key = e->key.keysym.sym;
    switch(key){
        case SDLK_ESCAPE:
            if(in_transition){
                break;
            }
            Room1_Leave();
            Room2_Leave();
            Pause_Load(&room_texDest);
            break;

#ifdef GAME_DEBUG
        case SDLK_F2:
            // Dev shortcut: plays the demo dialog (see mechanics/Dialog/dialog_scripts.c)
            DialogScripts_FelipeCall();
            break;

        case SDLK_F4:
            // Dev shortcut: the call with choices (Ana)
            DialogScripts_AnaCall();
            break;
#endif

        default:
            break;
    }
}

void Room_handleInput(SDL_Event*e)
{
    switch(e->type){
        case SDL_QUIT:
            Game_Quit();
            break;

        case SDL_KEYDOWN:
            Room_handleKeyDown(e);
            break;

        default:
            break;
    }
}


//
void Room1_handleInput(SDL_Event*e)
{
    size_t i;

    // An active dialog owns the mouse and the confirm keys
    if(Dialog_HandleInput(e)){
        return;
    }

    for(i=GPLAY_BUTTON_ARROW_ROOM1_BED ; i <= GPLAY_BUTTON_ARROW_ROOM1_MONITOR; ++i){
        Button_handleInput(play_buttons[i], e);
    }

    if(play_buttons[GPLAY_BUTTON_ARROW_ROOM1_BED]->state[INP_STATE_SELECTED]){
        Room1_Quit();
        Bed_Load(&room_texDest);
    }
    if(play_buttons[GPLAY_BUTTON_ARROW_ROOM1_BACK]->state[INP_STATE_SELECTED] && !in_transition){
        Room1_Leave();
        Room2_Load();
    }
    if(play_buttons[GPLAY_BUTTON_ARROW_ROOM1_MONITOR]->state[INP_STATE_SELECTED] && !in_transition){
        Room1_Leave();
        Monitor_Load();
    }

    Room_handleInput(e);
}

void Room1_displayButtons()
{
    Button *b;
    int x, y;
    double scale;

    // Adjust scale
    scale = Game_GetScaleH();

    // Bed button
    b = play_buttons[GPLAY_BUTTON_ARROW_ROOM1_BED];
    x = room_texDest.x + 2200 * room_texScaleW;
    y = room_texDest.y + 970 * room_texScaleH;
    Room_displayButtonArrow(b, x, y);

    // Look back button
    b = play_buttons[GPLAY_BUTTON_ARROW_ROOM1_BACK];
    x = room_texDest.x + room_texDest.w / 2;
    y = room_texDest.y + room_texDest.h - 40 * scale;
    Room_displayButtonArrow(b, x, y);

    // Check monitor
    b = play_buttons[GPLAY_BUTTON_ARROW_ROOM1_MONITOR];
    x = room_texDest.x + 929 * room_texScaleW;
    y = room_texDest.y + 330 * room_texScaleH;
    Room_displayButtonArrow(b, x, y);
}

void Room1_Render()
{
    SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
    SDL_RenderClear(r);

    //
    Room_displayBackground();
    Room1_displayButtons();
    Room_displayClock();
    Room_displayStatus();

    while(SDL_PollEvent(&e)){
        Room1_handleInput(&e);
    }
}

void Room1_Load()
{
    Play_TransitionInit(
        500,
        game_state,
        GAME_PLAY_STATE_VIEW_ROOM_1,
        GPLAY_TEX_ROOM_1
    );
}

void Room1_Leave()
{
}

void Room1_Quit()
{
}

//
void Room2_handleInput(SDL_Event*e)
{
    size_t i;

    if(Dialog_HandleInput(e)){
        return;
    }

    for(i=GPLAY_BUTTON_ARROW_ROOM2_BACK; i <= GPLAY_BUTTON_ARROW_ROOM2_DOOR; ++i){
        Button_handleInput(play_buttons[i], e);
    }

    if(play_buttons[GPLAY_BUTTON_ARROW_ROOM2_BACK]->state[INP_STATE_SELECTED] && !in_transition){
        Room2_Leave();
        Room1_Load();
    }

    Room_handleInput(e);
}

void Room2_displayButtons()
{
    Button *b;
    int x, y;
    double scale;

    // Adjust scale
    scale = Game_GetScaleH();

    // Back button
    b = play_buttons[GPLAY_BUTTON_ARROW_ROOM2_BACK];
    x = room_texDest.x + room_texDest.w / 2;
    y = room_texDest.y + room_texDest.h - 40 * scale;
    Room_displayButtonArrow(b, x, y);

    // Door button
    b = play_buttons[GPLAY_BUTTON_ARROW_ROOM2_DOOR];
    x = room_texDest.x + 330 * scale;
    y = room_texDest.y + 155 * scale;
    Room_displayButtonArrow(b, x, y);
}

void Room2_Render()
{
    SDL_SetRenderDrawColor(r, 255, 0, 0, 255); SDL_RenderClear(r);

    Room_displayBackground();
    Room2_displayButtons();
    Room_displayClock();
    Room_displayStatus();

    while(SDL_PollEvent(&e)){
        Room2_handleInput(&e);
    }
}

void Room2_Load()
{
    // game_playState  = GAME_PLAY_STATE_VIEW_ROOM_2;
    // gamePlay_tex    = GPLAY_TEX_ROOM_2;
    Play_TransitionInit(
        500,
        game_state,
        GAME_PLAY_STATE_VIEW_ROOM_2,
        GPLAY_TEX_ROOM_2
    );
}

void Room2_Leave()
{
}

void Room2_Quit()
{
}
