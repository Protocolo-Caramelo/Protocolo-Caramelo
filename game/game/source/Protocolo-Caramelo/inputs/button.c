#include"button.h"

static TextConfig *Button_tc_buffer;

Button* Button_Create(
    TextCache *cache, TextConfig* (*tc)[INP_STATES_MAX],
    char *str
    )
{
    Button *b;
    size_t i;

    b = malloc(sizeof(Button));
    //
    memset(b->state, 0, sizeof(b->state));
    for(i=0; i < INP_STATES_MAX; ++i){
        b->state_tc[i] = NULL;
        if((*tc)[i]){
            b->state_tc[i] = malloc(sizeof(*(b->state_tc[i])));
            *b->state_tc[i] = *(*tc)[i];
        }
    }
    //
    b->x = b->y = 0;
    b->text = Text_Create(
        cache,
        b->state_tc[INP_STATE_NEUTRAL],
        str
    );

    return b;
}

void Button_Destroy(Button**b)
{
    Text_Destroy(&(*b)->text);
    free(*b);
    *b = NULL;
}

//
void Button_Display(Button*b, int x, int y, void(*func)(Button*))
{
    Button_tc_buffer = 
        (b->state[INP_STATE_SELECTED] && b->state_tc[INP_STATE_SELECTED])? b->state_tc[INP_STATE_SELECTED]:
        (b->state[INP_STATE_HOVER] && b->state_tc[INP_STATE_HOVER])? b->state_tc[INP_STATE_HOVER]:
        b->state_tc[INP_STATE_NEUTRAL]
        ;

    if(func){
        func(b);
    }
    b->x = Button_tc_buffer->x = x;
    b->y = Button_tc_buffer->y = y;
    Text_DrawUTF8(
        b->text,
        NULL,
        Button_tc_buffer,
        NULL
    );
}

//
void Button_handleMouseButton(Button*b, SDL_Event*e, void(*func)(Button*, SDL_Event*))
{
    if(e->type != SDL_MOUSEBUTTONDOWN && e->type != SDL_MOUSEBUTTONUP){
        return;
    }

    int x, y;
    x = e->button.x;
    y = e->button.y;

    if(
        x >= (*Text_edges(b->text))[DRAW_EDGE_BORDER_XMIN] && x <= (*Text_edges(b->text))[DRAW_EDGE_BORDER_XMAX]
        && y >= (*Text_edges(b->text))[DRAW_EDGE_BORDER_YMIN] && y <= (*Text_edges(b->text))[DRAW_EDGE_BORDER_YMAX]
        && e->type == SDL_MOUSEBUTTONDOWN
      ){
        b->state[INP_STATE_SELECTED] = !b->state[INP_STATE_SELECTED];
    }

    if(e->type == SDL_MOUSEBUTTONUP){
        b->state[INP_STATE_SELECTED] = 0;
    }

    if(func){
        func(b, e);
    }
}

void Button_handleMouseMotion(Button*b, SDL_Event*e, void(*func)(Button*, SDL_Event*))
{
    if(e->type != SDL_MOUSEMOTION){
        return;
    }

    int x, y;
    x = e->button.x;
    y = e->button.y;

    b->state[INP_STATE_HOVER] = 
        x >= (*Text_edges(b->text))[DRAW_EDGE_BORDER_XMIN] && x <= (*Text_edges(b->text))[DRAW_EDGE_BORDER_XMAX]
        && y >= (*Text_edges(b->text))[DRAW_EDGE_BORDER_YMIN] && y <= (*Text_edges(b->text))[DRAW_EDGE_BORDER_YMAX]
        ;
    if(func){
        func(b, e);
    }
}

void Button_handleInput(Button*b, SDL_Event*e)
{
    Button_handleMouseButton(b, e, NULL);
    Button_handleMouseMotion(b, e, NULL);
}
