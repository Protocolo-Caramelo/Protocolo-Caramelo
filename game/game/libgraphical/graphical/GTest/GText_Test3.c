#if defined(COMPILATION_DEVQUOTE) || defined(COMPILATION_TOOLCHAIN_WINDOWS_DEVQUOTE)
#include"GTest.h"

#define PADDING_TOP 0x1
#define PADDING_RIGHT 0x2
#define PADDING_BOTTOM 0x4
#define PADDING_LEFT 0x8
//
#define BORDER_TOP 0x10
#define BORDER_RIGHT 0x20
#define BORDER_BOTTOM 0x40
#define BORDER_LEFT 0x80
//
#define MARGIN_TOP 0x100
#define MARGIN_RIGHT 0x200
#define MARGIN_BOTTOM 0x400
#define MARGIN_LEFT 0x800

static unsigned bitmask;

void draw_edges(
    Text*t, int t_width, int t_height,
    TextSpacing*ts,
    int xmax, int xmin, int ymax, int ymin,
    unsigned addr_top, unsigned addr_right, unsigned addr_bottom, unsigned addr_left
)
{
    int edgeT_x, edgeT_y;
    int edgeT_r;
    int edgeT_bw, edgeT_bh;

    if(bitmask & addr_top){
        edgeT_bh = ts->top + 1;
        edgeT_x = xmin / 2 + xmax / 2 - t_width / 2;
        edgeT_y = ymin;
        edgeT_r = 0;
        //
        Text_DrawUTF8(
            t,
            NULL,
            TextConfig_Create(
                .x = edgeT_x,
                .y = edgeT_y,
                .rotation = edgeT_r,
                .bx_height = edgeT_bh
            ),
            NULL
        );
    }
    if(bitmask & addr_right){
        edgeT_bh = ts->right + 1;
        // edgeT_x = xmax - t_width / 2 - MIN(edgeT_bh, t_height) / 2;
        edgeT_x = xmax - t_width / 2 - edgeT_bh / 2;
        edgeT_y = ymin / 2 + ymax / 2;
        edgeT_r = 90;
        //
        Text_DrawUTF8(
            t,
            NULL,
            TextConfig_Create(
                .x = edgeT_x,
                .y = edgeT_y,
                .rotation = edgeT_r,
                .bx_height = edgeT_bh
            ),
            NULL
        );
    }
    if(bitmask & addr_bottom){
        edgeT_bh = ts->bottom + 1;
        edgeT_x = xmin / 2 + xmax / 2 - t_width / 2;
        // edgeT_y = ymax - MIN(edgeT_bh, t_height);
        edgeT_y = ymax - edgeT_bh;
        edgeT_r = 180;
        //
        Text_DrawUTF8(
            t,
            NULL,
            TextConfig_Create(
                .x = edgeT_x,
                .y = edgeT_y,
                .rotation = edgeT_r,
                .bx_height = edgeT_bh
            ),
            NULL
        );
    }
    if(bitmask & addr_left){
        edgeT_bh = ts->left + 1;
        // edgeT_x = xmin - t_width / 2 + MIN(t_height, edgeT_bh) / 2;
        edgeT_x = xmin - t_width / 2 + edgeT_bh / 2;
        edgeT_y = ymin / 2 + ymax / 2;
        edgeT_r = 270;
        //
        Text_DrawUTF8(
            t,
            NULL,
            TextConfig_Create(
                .x = edgeT_x,
                .y = edgeT_y,
                .rotation = edgeT_r,
                .bx_height = edgeT_bh
            ),
            NULL
        );
    }
    // */
}

//
double angle_init = 0;
const double angle_rate = 5;
const double angle_ampl = 10;
SDL_Color color_init = (SDL_Color){.a = 255};
SDL_Color color_curr;
//
enum COLORS{
    RED,
    GREEN,
    BLUE,
    ALPHA
};
int8_t color_rate[4] = {10, 5, 0, 4};
int8_t color_rateCurr[4];

void alterText_1(Text*t, TextChar*tc)
{
    if(tc->chr_textIdx == 0){
        angle_init = fmod(angle_init + angle_rate, 360);
        //
        color_init.r = (Text_color(t)->r + color_init.r + color_rate[RED]);
        color_init.g = (Text_color(t)->g + color_init.g + color_rate[GREEN]);
        color_init.b = (Text_color(t)->b + color_init.b + color_rate[BLUE]);
        color_init.a = (Text_color(t)->a + color_init.a + color_rate[ALPHA]);
        //
        if(color_init.r + color_rate[RED] >= 255 || color_init.r + color_rate[RED] <= 0){
            color_rate[RED] *= -1;
        }
        if(color_init.g + color_rate[GREEN] >= 255 || color_init.g + color_rate[GREEN] <= 0){
            color_rate[GREEN] *= -1;
        }
        if(color_init.b + color_rate[BLUE] >= 255 || color_init.b + color_rate[BLUE] <= 0){
            color_rate[BLUE] *= -1;
        }
        if(color_init.a + color_rate[ALPHA] >= 255 || color_init.a + color_rate[ALPHA] <= 0){
            color_rate[ALPHA] *= -1;
        }
    }

    if(tc->chr_lineIdx == 0){
        color_curr = color_init;
        memcpy(color_rateCurr, color_rate, sizeof(color_rate) / sizeof(color_rate[0]));
    }

    if(color_curr.r + color_rateCurr[RED] >= 255 || color_curr.r + color_rateCurr[RED] <= 0){
        color_rateCurr[RED] *= -1;
    }
    if(color_curr.g + color_rateCurr[GREEN] >= 255 || color_curr.g + color_rateCurr[GREEN] <= 0){
        color_rateCurr[GREEN] *= -1;
    }
    if(color_curr.b + color_rateCurr[BLUE] >= 255 || color_curr.b + color_rateCurr[BLUE] <= 0){
        color_rateCurr[BLUE] *= -1;
    }
    if(color_curr.a + color_rateCurr[ALPHA] >= 255 || color_curr.a + color_rateCurr[ALPHA] <= 0){
        color_rateCurr[ALPHA] *= -1;
    }

    color_curr.r += color_rateCurr[RED];
    color_curr.g += color_rateCurr[GREEN];
    color_curr.b += color_rateCurr[BLUE];
    color_curr.a += color_rateCurr[ALPHA];
    //
    tc->y += angle_ampl * sin(DEG_TO_RAD(angle_init + angle_rate * tc->chr_lineIdx));
    //
    tc->color.r = color_curr.r;
    tc->color.g = color_curr.g;
    tc->color.b = color_curr.b;
    tc->color.a = color_curr.a;

    // printf("%lf %lf\n", tc->y, 40.0 * sin(DEG_TO_RAD(angle_init + angle_rate * tc->chr_lineIdx)));
    /*
    printf(
        "(%i %i %i %i)\n",
        tc->color.r,
        tc->color.g,
        tc->color.b,
        tc->color.a
    );
    // */
}

//
//*
void Text_Test3(char*fpath, unsigned ptsize, char*str)
{
    SDL_Window *w;
    SDL_Renderer *r;
    SDL_Event e;
    //
    TextCache *cache;
    Text *t;
    Margin t_margin;
    Border t_border;
    Padding t_padding;
    int t_edges[DRAW_EDGE_AMT];
    //
    TextConfig edges_config;
    Text *padding_t, *border_t, *margin_t;
    int paddingT_width, borderT_width, marginT_width;
    int paddingT_height, borderT_height, marginT_height;
    //
    unsigned time;
    //
    unsigned spacing_time;
    unsigned spacing_deltaTime;
    const unsigned spacing_valMax = 50;
    const unsigned spacing_timeDelay = 16;
    //
    Text *fps_t;
    double fps;
    unsigned fps_time;
    unsigned fps_deltaTime;
    const unsigned fps_timeDelay = 100;
    char fps_str[20] = "FPS: 60.00";

    if(graphical_init() != 0){

        exit(1);

    }
    //
    w = SDL_CreateWindow(
        "3nd test of Text struct",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1024,
        512,
        SDL_WINDOW_RESIZABLE
    );
    r = SDL_CreateRenderer(
        w,
        -1,
        SDL_RENDERER_ACCELERATED
    );
    //
    cache = TextCache_Create(r, fpath, ptsize);
    if(!cache){
        exit(1);
    }
    t = Text_Create(cache);
    Text_UpdateConfig(t, TextConfig_Create(
        .color = (SDL_Color){255, 255, 0, 255},
        .color_bg = (SDL_Color){255, 0, 255, 255},
        .scale_width = 1 / 2.0,
        .scale_height = 1 / 2.0,
        .wrap = 434,
        .x = 0,
        .y = 0,

        .func_alterText = &alterText_1
    ));
    t_margin = *Text_margin(t);
    t_border = *Text_border(t);
    t_padding = *Text_padding(t);
    //
    edges_config = *TextConfig_Create(
        .scale_width = 1 / 1.5,
        .scale_height = 1 / 1.5,
        .color = (SDL_Color){255, 255, 255, 255},
    );
    padding_t = Text_Create(cache);
    border_t = Text_Create(cache);
    margin_t = Text_Create(cache);
    Text_UpdateConfig(padding_t, &edges_config);
    Text_UpdateConfig(border_t, &edges_config);
    Text_UpdateConfig(margin_t, &edges_config);
    Text_SetUTF8(padding_t, "Padding");
    Text_SetUTF8(border_t, "Border");
    Text_SetUTF8(margin_t, "Margin");
    Text_MeasureUTF8(padding_t, &paddingT_width, &paddingT_height);
    Text_MeasureUTF8(border_t, &borderT_width, &borderT_height);
    Text_MeasureUTF8(margin_t, &marginT_width, &marginT_height);
    //
    fps_t = Text_Create(cache);
    Text_UpdateConfig(fps_t, TextConfig_Create(
        .scale_width = 1 / 2.0,
        .scale_height = 1 / 2.0,
        .wrap = 0,
        .color = (SDL_Color){0, 0, 0, 255},
        .y = 0,
        .x = 734
        ));
    //
    time = spacing_time = fps_time = SDL_GetTicks();
    
    //
    t_margin.color = (SDL_Color){255, 0, 0, 255};
    t_border.color = (SDL_Color){0, 255, 0, 255};
    t_padding.color = (SDL_Color){0, 0, 255, 255};
    bitmask = (
        0
        | MARGIN_TOP | MARGIN_RIGHT | MARGIN_BOTTOM | MARGIN_LEFT
        | BORDER_TOP | BORDER_RIGHT | BORDER_BOTTOM | BORDER_LEFT
        // | PADDING_TOP | PADDING_RIGHT | PADDING_BOTTOM | PADDING_LEFT
    );

    while(1){
        SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
        SDL_RenderClear(r);
        //
        spacing_deltaTime = SDL_GetTicks() - spacing_time;
        fps_deltaTime = SDL_GetTicks() - fps_time;
        // printf("%i\n", bitmask);

        // fps
        if(fps_deltaTime >= fps_timeDelay){
            sprintf(fps_str, "FPS: %.2lf", fps);
        }

        // bitmask
        // printf("%i %i\n", bitmask, t_padding->bottom);
        if(bitmask & PADDING_BOTTOM && spacing_deltaTime >= spacing_timeDelay){
            ++t_padding.bottom;
        }
        if(bitmask & PADDING_TOP && spacing_deltaTime >= spacing_timeDelay){
            ++t_padding.top;
        }
        if(bitmask & PADDING_RIGHT && spacing_deltaTime >= spacing_timeDelay){
            ++t_padding.right;
        }
        if(bitmask & PADDING_LEFT && spacing_deltaTime >= spacing_timeDelay){
            ++t_padding.left;
        }

        //*
        if(bitmask & BORDER_BOTTOM && spacing_deltaTime >= spacing_timeDelay){
            ++t_border.bottom;
        }
        if(bitmask & BORDER_TOP && spacing_deltaTime >= spacing_timeDelay){
            ++t_border.top;
        }
        if(bitmask & BORDER_RIGHT && spacing_deltaTime >= spacing_timeDelay){
            ++t_border.right;
        }
        if(bitmask & BORDER_LEFT && spacing_deltaTime >= spacing_timeDelay){
            ++t_border.left;
        }

        if(bitmask & MARGIN_BOTTOM && spacing_deltaTime >= spacing_timeDelay){
            ++t_margin.bottom;
        }
        if(bitmask & MARGIN_TOP && spacing_deltaTime >= spacing_timeDelay){
            ++t_margin.top;
        }
        if(bitmask & MARGIN_RIGHT && spacing_deltaTime >= spacing_timeDelay){
            ++t_margin.right;
        }
        if(bitmask & MARGIN_LEFT && spacing_deltaTime >= spacing_timeDelay){
            ++t_margin.left;
        }
        // /

        // Draw fps
        //*
        Text_DrawUTF8(
            fps_t,
            fps_str,
            NULL,
            NULL
        );
        // */

        //* Draw text
        Text_DrawUTF8(
            t,
            str,
            TextConfig_Create(
                .x = 0,
                .y = 0,
                .padding = t_padding,
                .border = t_border,
                .margin = t_margin,
            ),
            &t_edges
        );

        //* Draw edges
        draw_edges(
            padding_t, paddingT_width, paddingT_height,
            &t_padding,
            t_edges[DRAW_EDGE_PADDING_XMAX], t_edges[DRAW_EDGE_PADDING_XMIN],
            t_edges[DRAW_EDGE_PADDING_YMAX], t_edges[DRAW_EDGE_PADDING_YMIN],
            PADDING_TOP, PADDING_RIGHT, PADDING_BOTTOM, PADDING_LEFT
        );
        draw_edges(
            border_t, borderT_width, borderT_height,
            &t_border,
            t_edges[DRAW_EDGE_BORDER_XMAX], t_edges[DRAW_EDGE_BORDER_XMIN],
            t_edges[DRAW_EDGE_BORDER_YMAX], t_edges[DRAW_EDGE_BORDER_YMIN],
            BORDER_TOP, BORDER_RIGHT, BORDER_BOTTOM, BORDER_LEFT
        );
        draw_edges(
            margin_t, marginT_width, marginT_height,
            &t_margin,
            t_edges[DRAW_EDGE_MARGIN_XMAX], t_edges[DRAW_EDGE_MARGIN_XMIN],
            t_edges[DRAW_EDGE_MARGIN_YMAX], t_edges[DRAW_EDGE_MARGIN_YMIN],
            MARGIN_TOP, MARGIN_RIGHT, MARGIN_BOTTOM, MARGIN_LEFT
        );
        // */

        // Check events
        while(SDL_PollEvent(&e)){
            switch(e.type){
                case SDL_QUIT:
                    SDL_DestroyWindow(w);
                    SDL_DestroyRenderer(r);
                    SDL_Quit();
                    //
                    Text_Destroy(&t);
                    Text_Destroy(&fps_t);
                    Text_Destroy(&padding_t);
                    Text_Destroy(&border_t);
                    Text_Destroy(&margin_t);
                    TextCache_Destroy(&cache);
                    //
                    exit(0);

                default:
                    break;
            }
        }

        SDL_RenderPresent(r);
        SDL_Delay(16);
        //
        if(spacing_deltaTime >= spacing_timeDelay){
            spacing_time = SDL_GetTicks();
        }
        if(fps_deltaTime >= fps_timeDelay){
            fps_time = SDL_GetTicks();
        }

        if(
            t_padding.bottom >= spacing_valMax
            || t_padding.top >= spacing_valMax
            || t_padding.right >= spacing_valMax
            || t_padding.left >= spacing_valMax

            || t_border.bottom >= spacing_valMax
            || t_border.top >= spacing_valMax
            || t_border.right >= spacing_valMax
            || t_border.left >= spacing_valMax

            || t_margin.bottom >= spacing_valMax
            || t_margin.top >= spacing_valMax
            || t_margin.right >= spacing_valMax
            || t_margin.left >= spacing_valMax
          ){
            t_padding.bottom =
                t_padding.top =
                t_padding.right = 
                t_padding.left = 
                //
                t_border.bottom =
                t_border.top =
                t_border.right = 
                t_border.left = 
                //
                t_margin.bottom =
                t_margin.top =
                t_margin.right = 
                t_margin.left = 0;

            ++bitmask;
        }

        fps = 1000.0 / (SDL_GetTicks() - time);
        time = SDL_GetTicks();
        bitmask %= (MARGIN_LEFT << 1);
        if(bitmask == 0){
            bitmask = 1;
        }
    }
}
// */

/*
void Text_Test3(char*fpath, unsigned ptsize, char*str)
{
    SDL_Window *w;
    SDL_Renderer *r;
    SDL_Event e;

    w = SDL_CreateWindow(
        "3nd test of Text struct",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1024,
        512,
        SDL_WINDOW_RESIZABLE
    );
    r = SDL_CreateRenderer(
        w,
        -1,
        SDL_RENDERER_ACCELERATED
    );

    while(1){
        SDL_SetRenderDrawColor(r, 255, 255, 255, 0);
        SDL_RenderClear(r);

        while(SDL_PollEvent(&e)){
            switch(e.type){
                case SDL_QUIT:
                    SDL_DestroyWindow(w);
                    SDL_DestroyRenderer(r);
                    SDL_Quit();
                    exit(0);
                    break;

                default:
                    break;
            }
        }

        SDL_RenderPresent(r);
        SDL_Delay(16);
    }
}
// */

#endif
