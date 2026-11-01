#include"room.h"

static SDL_Rect room_texDest;
static SDL_Rect room_texDestRaw;
static double room_texScaleW;
static double room_texScaleH;

void Room_displayButtonArrow(Button*b, int x, int y)
{
    Game_PlayDisplayButtonArrow(b, &room_texDestRaw, x, y);
}

//
void Room1_handleInput(SDL_Event*e)
{
    size_t i;
    for(i=GAMEPLAY_BUTTON_ARROW_ROOM1_BED ; i <= GAMEPLAY_BUTTON_ARROW_ROOM1_MONITOR; ++i){
        Button_handleInput(play_buttons[i], e);
    }

    if(play_buttons[GAMEPLAY_BUTTON_ARROW_ROOM1_BED]->state[INP_STATE_SELECTED]){
        printf("Feature not avaible yet\n");
    }
    if(play_buttons[GAMEPLAY_BUTTON_ARROW_ROOM1_BACK]->state[INP_STATE_SELECTED]){
        gamePlay_state  = GAMEPLAY_STATE_VIEW_ROOM_2;
        gamePlay_tex    = GAMEPLAY_TEX_ROOM_2;
    }
    if(play_buttons[GAMEPLAY_BUTTON_ARROW_ROOM1_MONITOR]->state[INP_STATE_SELECTED]){
        gamePlay_state  = GAMEPLAY_STATE_VIEW_MONITOR;
        gamePlay_tex    = GAMEPLAY_TEX_MONITOR_POWER_BUTTON_UNSELECTED;
    }

    switch(e->type){
        case SDL_QUIT:
            Game_PlayQuit();
            break;

        default:
            break;
    }
}

void Room1_displayButtons()
{
    Button *b;
    int x, y;
    double scale;

    // Adjust scale
    scale = Game_GetScaleH();

    // Bed button
    b = play_buttons[GAMEPLAY_BUTTON_ARROW_ROOM1_BED];
    x = room_texDest.x + 2200 * room_texScaleW;
    y = room_texDest.y + 1120 * room_texScaleH;
    Room_displayButtonArrow(b, x, y);

    // Look back button
    b = play_buttons[GAMEPLAY_BUTTON_ARROW_ROOM1_BACK];
    // x = room_texDest.x + room_texDest.w / 2 - (*Text_edges(b->text))[DRAW_WIDTH] / 2;
    // y = room_texDest.y + room_texDest.h - ((*Text_edges(b->text))[DRAW_HEIGHT] - 30) * scale;
    x = room_texDest.x + room_texDest.w / 2;
    y = room_texDest.y + room_texDest.h - 40 * scale;
    Room_displayButtonArrow(b, x, y);

    // Check monitor
    b = play_buttons[GAMEPLAY_BUTTON_ARROW_ROOM1_MONITOR];
    x = room_texDest.x + 924 * room_texScaleW;
    y = room_texDest.y + 397 * room_texScaleH;
    Room_displayButtonArrow(b, x, y);
}

void Room1_Render()
{
    //
    SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
    SDL_RenderClear(r);

    while(SDL_PollEvent(&e)){
        Room1_handleInput(&e);
    }

    //
    Game_PlayDisplayBackground(
        gamePlay_tex,
        &room_texDest, &room_texDestRaw,
        &room_texScaleW, &room_texScaleH
    );
    Room1_displayButtons();
}

//
void Room2_handleInput(SDL_Event*e)
{
    size_t i;
    for(i=GAMEPLAY_BUTTON_ARROW_ROOM2_BACK; i <= GAMEPLAY_BUTTON_ARROW_ROOM2_DOOR; ++i){
        Button_handleInput(play_buttons[i], e);
    }

    if(play_buttons[GAMEPLAY_BUTTON_ARROW_ROOM2_BACK]->state[INP_STATE_SELECTED]){
        gamePlay_state  = GAMEPLAY_STATE_VIEW_ROOM_1;
        gamePlay_tex    = GAMEPLAY_TEX_ROOM_1;
    }

    switch(e->type){
        case SDL_QUIT:
            Game_PlayQuit();
            break;

        default:
            break;
    }
}

void Room2_displayButtons()
{
    Button *b;
    int x, y;
    double scale;

    // Adjust scale
    scale = Game_GetScaleH();

    // Back button
    b = play_buttons[GAMEPLAY_BUTTON_ARROW_ROOM2_BACK];
    // x = room_texDest.x + room_texDest.w / 2 - (*Text_edges(b->text))[DRAW_WIDTH] / 2;
    x = room_texDest.x + room_texDest.w / 2;
    // y = room_texDest.y + room_texDest.h - ((*Text_edges(b->text))[DRAW_HEIGHT] - 30) * scale;
    y = room_texDest.y + room_texDest.h - 100 * room_texScaleH;
    Room_displayButtonArrow(b, x, y);

    // Door button
    b = play_buttons[GAMEPLAY_BUTTON_ARROW_ROOM2_DOOR];
    x = room_texDest.x + 330 * scale;
    y = room_texDest.y + 155 * scale;
    Room_displayButtonArrow(b, x, y);
}

void Room2_Render()
{
    SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
    SDL_RenderClear(r);

    while(SDL_PollEvent(&e)){
        Room2_handleInput(&e);
    }

    Game_PlayDisplayBackground(
        gamePlay_tex,
        &room_texDest, &room_texDestRaw,
        &room_texScaleW, &room_texScaleH 
    );
    Room2_displayButtons();
}
