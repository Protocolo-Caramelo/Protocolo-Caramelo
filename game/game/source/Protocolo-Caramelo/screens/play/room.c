#include"room.h"

static SDL_Rect room_texDest;
static SDL_Rect room_texDestRaw;

void Room_displayButtonArrow(Button*b, int x, int y)
{
    double scale;
    //
    int b_borderL, b_borderR;
    int b_borderLPrev, b_borderRPrev;
    double scale_width, scale_height;
    double scale_widthPrev, scale_heightPrev;
    char *str_prev;
    //
    size_t i;

    //
    b_borderLPrev = b_borderL       = b->state_tc[INP_STATE_NEUTRAL]->border.left;
    b_borderRPrev = b_borderR       = b->state_tc[INP_STATE_NEUTRAL]->border.right;
    scale_widthPrev = scale_width   = b->state_tc[INP_STATE_NEUTRAL]->scale_width;
    scale_heightPrev = scale_height = b->state_tc[INP_STATE_NEUTRAL]->scale_height;
    str_prev                        = malloc(sizeof(Text_GetLenUTF8(b->text)));

    if(w_width / (double)w_height < room_texDestRaw.w / (double)room_texDestRaw.h){
        scale = w_width / (double)ELEMENT_WINDOW_W;
    }
    else {
        scale = w_height / (double)ELEMENT_WINDOW_H;
    }
    b_borderR    *= scale;
    b_borderL    *= scale;
    scale_width  *= scale;
    scale_height *= scale;
    utf8cpy(str_prev, *Text_raw(b->text));

    for(i=0; i < INP_STATES_MAX; ++i){
        b->state_tc[i]->border.left = b_borderL;
        b->state_tc[i]->border.right = b_borderR;
        //
        b->state_tc[i]->scale_width = scale_width;
        b->state_tc[i]->scale_height = scale_height;
        //
    }
    if(!b->state[INP_STATE_SELECTED] && !b->state[INP_STATE_HOVER]){
        // free(*Text_raw(b->text));
        // *Text_raw(b->text) = NULL;
    }

    //
    Button_Display(
        b,
        x,
        y,
        NULL
    );

    //
    for(i=0; i < INP_STATES_MAX; ++i){
        b->state_tc[i]->border.left = b_borderLPrev;
        b->state_tc[i]->border.right = b_borderRPrev;
        //
        b->state_tc[i]->scale_width = scale_widthPrev;
        b->state_tc[i]->scale_height = scale_heightPrev;
    }
    Text_SetUTF8(b->text, str_prev);
    free(str_prev);
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
        gamePlay_tex    = GAMEPLAY_TEX_MONITOR;
    }

    switch(e->type){
        case SDL_QUIT:
            Game_Quit();
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
    scale = (w_height / (double)ELEMENT_WINDOW_H);

    // Bed button
    b = play_buttons[GAMEPLAY_BUTTON_ARROW_ROOM1_BED];
    x = room_texDest.x + 10 * scale;
    y = room_texDest.y + room_texDest.h - 270 * scale;
    Room_displayButtonArrow(b, x, y);

    // Look back button
    b = play_buttons[GAMEPLAY_BUTTON_ARROW_ROOM1_BACK];
    x = room_texDest.x + room_texDest.w / 2 - (*Text_edges(b->text))[DRAW_WIDTH] / 2;
    y = room_texDest.y + room_texDest.h - ((*Text_edges(b->text))[DRAW_HEIGHT] - 30) * scale;
    Room_displayButtonArrow(b, x, y);

    // Check monitor
    b = play_buttons[GAMEPLAY_BUTTON_ARROW_ROOM1_MONITOR];
    x = room_texDest.x + room_texDest.w - 465 * scale;
    y = room_texDest.y + 90 * scale;
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
    Game_PlayDisplayBackground(GAMEPLAY_TEX_ROOM_1, &room_texDest, &room_texDestRaw);
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
            Game_Quit();
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
    scale = (w_height / (double)ELEMENT_WINDOW_H);

    // Back button
    b = play_buttons[GAMEPLAY_BUTTON_ARROW_ROOM2_BACK];
    x = room_texDest.x + room_texDest.w / 2 - (*Text_edges(b->text))[DRAW_WIDTH] / 2;
    y = room_texDest.y + room_texDest.h - ((*Text_edges(b->text))[DRAW_HEIGHT] - 30) * scale;
    Room_displayButtonArrow(b, x, y);

    // Door button
    b = play_buttons[GAMEPLAY_BUTTON_ARROW_ROOM2_DOOR];
    x = room_texDest.x + 270 * scale;
    y = room_texDest.y + 110 * scale;
    Room_displayButtonArrow(b, x, y);
}

void Room2_Render()
{
    SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
    SDL_RenderClear(r);

    while(SDL_PollEvent(&e)){
        Room2_handleInput(&e);
    }

    Game_PlayDisplayBackground(GAMEPLAY_TEX_ROOM_2, &room_texDest, &room_texDestRaw);
    Room2_displayButtons();
}
