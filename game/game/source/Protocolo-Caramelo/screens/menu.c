#include"menu.h"

static int state_prev[INP_STATES_MAX];
static int state_changed;

void Menu_displayButtonCentered(Button*b, int x, int y)
{
    Text *t;
    int t_w, t_h;
    //
    int p_w, p_h;
    int p_t, p_b;
    int p_r, p_l;
    //
    size_t i;

    t = b->text;
    t_w = (*Text_edges(t))[DRAW_TEXT_WIDTH];
    t_h = (*Text_edges(t))[DRAW_TEXT_HEIGHT];
    //
    p_w = MENU_BUTTON_PADDING_NEUTRAL_W;
    p_h = MENU_BUTTON_PADDING_NEUTRAL_H;
    if(b->state[INP_STATE_SELECTED]){
        p_w = MENU_BUTTON_PADDING_SELECTED_W;
        p_h = MENU_BUTTON_PADDING_SELECTED_H;
    }
    else if(b->state[INP_STATE_HOVER]){
        p_w = MENU_BUTTON_PADDING_HOVER_W;
        p_h = MENU_BUTTON_PADDING_HOVER_H;
    }

    p_r = p_l = p_w / 2 - t_w / 2;
    p_t = p_b = p_h / 2 - t_h / 2;

    for(i=0; i < INP_STATES_MAX; ++i){
        if(!b->state_tc[i]){
            continue;
        }
        b->state_tc[i]->padding.right = p_r;
        b->state_tc[i]->padding.left = p_l;
        b->state_tc[i]->padding.top = p_t;
        b->state_tc[i]->padding.bottom = p_b;
    }

    x =
        (!state_changed)?
        x
        - *Text_marginLeft(t) - *Text_borderLeft(t) - *Text_paddingLeft(t)
        : *Text_x(t)
        ;
    y =
        (!state_changed)?
        y
        - *Text_marginTop(t) - *Text_borderTop(t) - *Text_paddingTop(t)
        - *Text_marginBottom(t) - *Text_borderBottom(t) - *Text_paddingBottom(t)
        : *Text_y(t)
        ;

    Button_Display(b, x, y, NULL);
}

void Menu_handleEvent(SDL_Event*e)
{
    size_t i, j;
    for(i=0; i < MENU_BUTTONS_MAX; ++i){
        memcpy(state_prev, menu_buttons[i]->state, sizeof(state_prev));
        Button_handleInput(menu_buttons[i], e);
        for(j=0; j < INP_STATES_MAX && !state_changed; ++j){
            state_changed = state_prev[j] != menu_buttons[i]->state[j];
        }
    }

    if(menu_buttons[MENU_BUTTON_EXIT]->state[INP_STATE_SELECTED]){
        Game_Quit();
        return;
    }

    if(menu_buttons[MENU_BUTTON_INIT]->state[INP_STATE_SELECTED]){
        // gameState = GAME_STATE_PLAYING;
        // Game_PlayInit();
    }

    switch(e->type){
        case SDL_QUIT:
            Game_Quit();
            break;

        default:
            break;
    }
}

void Menu_displayMainButtons()
{
    int (*edges)[DRAW_EDGE_AMT];
    //
    Button *b;
    int b_x, b_y;
    //
    int remain_buttons;
    int remain_w;
    //
    size_t i, j;
    //
    Text *t;

    edges = Text_edges(menu_tTitle[MENU_TITLE_COUNT - 1]);

    //
    for(i=MENU_BUTTON_INIT; i <= MENU_BUTTON_CONFIG; ++i){
    // for(i=MENU_BUTTON_INIT; i <= MENU_BUTTON_INIT; ++i){
        b = menu_buttons[i];
        t = b->text;

        // Draw Button
        //*
        b_x =
            (!state_changed)?
            (*edges)[DRAW_EDGE_XMIN] + (*edges)[DRAW_WIDTH] / 2 - (*Text_edges(t))[DRAW_TEXT_WIDTH] / 2 
            + ((i == MENU_BUTTON_INIT)? MENU_BUTTON_SPACING_TITLE_X: MENU_BUTTON_SPACING_X)
            : *Text_x(t)
            ;
        b_y =
            (!state_changed)?
            (*edges)[DRAW_EDGE_YMAX] 
            + MENU_BUTTON_SPACING_Y
            + ((i == MENU_BUTTON_INIT)? MENU_BUTTON_SPACING_TITLE_Y: MENU_BUTTON_SPACING_Y)
            : *Text_y(t)
            ;
        // */

        Menu_displayButtonCentered(b, b_x, b_y);
        edges = Text_edges(b->text);
        //
    }

    // Display about and exit buttons
    //*
    b_x = 
        (*Text_edges(t))[DRAW_EDGE_XMIN] + (*Text_edges(t))[DRAW_WIDTH] / 2
        ;
    b_y =
        (*Text_edges(t))[DRAW_EDGE_YMAX] 
        + MENU_BUTTON_SPACING_BUTTONS_BOTTOM_Y
        ;
    remain_buttons = MENU_BUTTONS_MAX - i - 1;
    remain_w = 0;
    for(j=i; j < MENU_BUTTONS_MAX; ++j){
        t = menu_buttons[i]->text;
        remain_w +=
            (*Text_edges(t))[DRAW_WIDTH]
            + MENU_BUTTON_SPACING_BUTTONS_BOTTOM_X
            // + *Text_paddingLeft(t) + *Text_borderLeft(t) + *Text_marginLeft(t)
                ;
    }

    b_x -= remain_w / 2;
    for(; i < MENU_BUTTONS_MAX; ++i){
        b = menu_buttons[i];
        t = b->text;

        Menu_displayButtonCentered(
            b
            , b_x + *Text_paddingLeft(t) + *Text_borderLeft(t) + *Text_marginLeft(t)
            , b_y
        );
        b_x +=
            (*Text_edges(t))[DRAW_WIDTH]
            + MENU_BUTTON_SPACING_BUTTONS_BOTTOM_X
            ;
    }
    // */
}

void Menu_Render()
{
    int x, y;
    size_t i;

    SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
    SDL_RenderClear(r);

    //
    while(SDL_PollEvent(&e)){
        Menu_handleEvent(&e);
    }

    //
    SDL_GetWindowSize(w, &w_width, &w_height);

    y = 50;
    for(i=0; i < MENU_TITLE_COUNT; ++i){
        x = w_width / 2 - (*Text_edges(menu_tTitle[i]))[DRAW_WIDTH] / 2;
        Text_DrawUTF8(
            menu_tTitle[i],
            NULL,
            TextConfig_Create(
                .x = x,
                .y = y,
            ),
            NULL
        );
        y = (*Text_edges(menu_tTitle[i]))[DRAW_EDGE_YMAX] + 10;
    }
    Menu_displayMainButtons();

    state_changed = 0;
}
