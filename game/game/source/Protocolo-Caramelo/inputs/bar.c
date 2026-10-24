#include"bar.h"

static char BarInput_buffer[256];
static TextConfig *BarInput_tc_buffer;

BarInput* BarInput_Create(
    TextCache*cache, TextConfig* (*tc)[INP_STATES_MAX],
    char *fmt,
    double min, double max,
    int w, int h,
    SDL_Color c_init, SDL_Color c_end
    )
{
    BarInput *bi;
    size_t i;

    bi = malloc(sizeof(BarInput));

    memset(bi->state, 0, sizeof(bi->state));
    for(i=0; i < INP_STATES_MAX; ++i){
        bi->state_tc[i] = NULL;
        if((*tc)[i]){
            bi->state_tc[i] = malloc(sizeof(*(bi->state_tc[i])));
            bi->state_tc[i] = (*tc)[i];
        }
    }
    //
    bi->rg_min = min;
    bi->rg_max = max;
    //
    bi->w = bi->bar_w = w;
    bi->h = bi->bar_h = h;
    //
    bi->c_init = c_init;
    bi->c_end = c_end;
    //
    bi->fmt = malloc(sizeof(strlen(fmt) + 1));
    strcpy(bi->fmt, fmt);
    //
    bi->text = Text_Create(
        cache,
       bi->state_tc[INP_STATE_NEUTRAL]
    );
    *Text_borderLeft(bi->text) = bi->w;
    *Text_borderTop(bi->text) = bi->h;

    return bi;
}

void BarInput_Destroy(BarInput**bi)
{
    size_t i;

    Text_Destroy(&(*bi)->text);
    free((*bi)->fmt);
    free(*bi);

    *bi = NULL;
}

void BarInput_Display(BarInput*bi, int x, int y, void(*func)(BarInput*))
{
    BarInput_tc_buffer = 
        (bi->state[INP_STATE_SELECTED] && bi->state_tc[INP_STATE_SELECTED])? bi->state_tc[INP_STATE_SELECTED]:
        (bi->state[INP_STATE_HOVER] && bi->state_tc[INP_STATE_SELECTED])? bi->state_tc[INP_STATE_HOVER]:
        bi->state_tc[INP_STATE_NEUTRAL]
        ;
    BarInput_tc_buffer->padding.left = *Text_paddingLeft(bi->text);
    BarInput_tc_buffer->padding.top = *Text_paddingTop(bi->text);
    //
    BarInput_tc_buffer->border.left = *Text_borderLeft(bi->text);
    BarInput_tc_buffer->border.top = *Text_borderTop(bi->text);
    //
    BarInput_tc_buffer->x = x;
    BarInput_tc_buffer->y= y;

    BarInput_buffer[0] = '\0';
    sprintf(BarInput_buffer, bi->fmt, BarInput_GetVal(bi));

    if(func){
        func(bi);
    }
    Text_DrawUTF8(
        bi->text,
        BarInput_buffer,
        BarInput_tc_buffer,
        NULL
    );
}

double BarInput_GetVal(BarInput*bi)
{
    double val = bi->bar_w / (double)bi->w * (bi->rg_max - bi->rg_min) + bi->rg_min;
    return (val == val)? val: 0;
}

void BarInput_SetVaL(BarInput*bi, int val)
{
    *Text_borderLeft(bi->text) = MIN(MAX((val - bi->rg_min) / (double)bi->rg_max * bi->w, 0), bi->w);
    *Text_paddingLeft(bi->text) = bi->w - *Text_borderLeft(bi->text);

    *Text_borderTop(bi->text) = bi->h;
    *Text_paddingTop(bi->text) = 0;
    if(*Text_borderLeft(bi->text)){
        *Text_paddingTop(bi->text) = bi->h;
        *Text_borderTop(bi->text) = 0;
    }
    bi->bar_w = *Text_borderLeft(bi->text);
    bi->bar_h = bi->h;
}

//
void BarInput_handleMouseButton(BarInput*bi, SDL_Event*e, void(*func)(BarInput*, SDL_Event*))
{
    if(e->type != SDL_MOUSEBUTTONDOWN && e->type != SDL_MOUSEBUTTONUP){
        return;
    }

    int x, y;
    x = e->button.x;
    y = e->button.y;

    bi->state[INP_STATE_SELECTED] = 
        x >= (*Text_edges(bi->text))[DRAW_EDGE_BORDER_XMIN] && x <= (*Text_edges(bi->text))[DRAW_EDGE_BORDER_XMAX]
        && y >= (*Text_edges(bi->text))[DRAW_EDGE_BORDER_YMIN] && y <= (*Text_edges(bi->text))[DRAW_EDGE_BORDER_YMAX]
        ;
    if(e->type == SDL_MOUSEBUTTONUP){
        bi->state[INP_STATE_SELECTED] = 0;
    }
    /*
    printf(
        "(%i %i) (%i %i)\n",
        (*Text_edges(bi->text))[DRAW_EDGE_TEXT_XMIN],
        (*Text_edges(bi->text))[DRAW_EDGE_TEXT_XMAX],
        (*Text_edges(bi->text))[DRAW_EDGE_TEXT_YMIN],
        (*Text_edges(bi->text))[DRAW_EDGE_TEXT_YMAX]
    );
    // */
    if(func){
        func(bi, e);
    }
}

void BarInput_handleMouseMotion(BarInput*bi, SDL_Event*e, void(*func)(BarInput*, SDL_Event*))
{
    if(e->type != SDL_MOUSEMOTION && e->type != SDL_MOUSEBUTTONDOWN){
        return;
    }
    //
    int x, y;
    int r_x, r_y;
    int r_xBorder, r_xPadding;
    double t_sin, t_cos;

    x = (e->type == SDL_MOUSEMOTION)? e->motion.x: e->button.x;
    y = (e->type == SDL_MOUSEMOTION)? e->motion.y: e->button.y;
    bi->state[INP_STATE_HOVER] = 
        x >= (*Text_edges(bi->text))[DRAW_EDGE_BORDER_XMIN] && x <= (*Text_edges(bi->text))[DRAW_EDGE_BORDER_XMAX]
        && y >= (*Text_edges(bi->text))[DRAW_EDGE_BORDER_YMIN] && y <= (*Text_edges(bi->text))[DRAW_EDGE_BORDER_YMAX]
        ;

    //
    if(bi->state[INP_STATE_SELECTED]){
        bi->state[INP_STATE_SELECTED] = 1;
        //
        //
        t_sin = sin(DEG_TO_RAD(*Text_rotation(bi->text)));
        t_cos = cos(DEG_TO_RAD(*Text_rotation(bi->text)));
        //
        r_x = x * t_cos + y * t_sin;
        r_y = y * t_cos + x * t_sin;
        r_xBorder =
            (*Text_edges(bi->text))[DRAW_EDGE_BORDER_XMIN] * t_cos
            + (*Text_edges(bi->text))[DRAW_EDGE_BORDER_YMIN] * t_sin;
        r_xPadding =
            (*Text_edges(bi->text))[DRAW_EDGE_TEXT_XMIN] * t_cos
            + (*Text_edges(bi->text))[DRAW_EDGE_TEXT_YMIN] * t_sin;
        //
        *Text_borderLeft(bi->text) = MIN(MAX(r_x - r_xBorder, 0), bi->w);
        // *Text_borderLeft(bi->text) = MIN(MAX(r_xBorder - r_x, 0), bi->w);
        *Text_paddingLeft(bi->text) = bi->w - *Text_borderLeft(bi->text);
        *Text_paddingTop(bi->text) = bi->h;
        *Text_borderTop(bi->text) = 0;
        // printf("%i (%i %i) (%i %i)\n", r_x, r_xPadding, r_xBorder, MIN(MAX(r_xPadding - r_x, 0), bi->w), MIN(MAX(r_x - r_xBorder, 0), bi->w));
        // printf("%i %i\n", (*Text_edges(bi->text))[DRAW_EDGE_BORDER_XMIN], (*Text_edges(bi->text))[DRAW_EDGE_TEXT_XMIN]);
        /*
        printf(
            "(%i %i) (%i %i)\n",
            (*Text_edges(bi->text))[DRAW_EDGE_TEXT_XMIN],
            (*Text_edges(bi->text))[DRAW_EDGE_TEXT_XMAX],
            (*Text_edges(bi->text))[DRAW_EDGE_TEXT_YMIN],
            (*Text_edges(bi->text))[DRAW_EDGE_TEXT_YMAX]
        );
        // */
        if(!*Text_paddingLeft(bi->text)){
            *Text_borderTop(bi->text) = bi->h;
            *Text_paddingTop(bi->text) = 0;
        }
        bi->bar_w = *Text_borderLeft(bi->text);
        bi->bar_h = bi->h;
    }
    if(func){
        func(bi, e);
    }
}

void BarInput_handleInput(BarInput*bi, SDL_Event *e)
{
    BarInput_handleMouseButton(bi, e, NULL);
    BarInput_handleMouseMotion(bi, e, NULL);
}
