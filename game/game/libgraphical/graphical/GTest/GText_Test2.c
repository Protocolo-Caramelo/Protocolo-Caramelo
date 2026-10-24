#if defined(COMPILATION_DEVQUOTE) || defined(COMPILATION_TOOLCHAIN_WINDOWS_DEVQUOTE)
#include"GTest.h"

//
// Alter functions
//
typedef enum AngleState {
    ANGLE_REDUCE,
    ANGLE_INCREASE,
} AngleState;

double radius;
double angle;
double angle_init;
//
double cY, cX;
int cY_min, cX_min;
//
int position;
//
double minus;
double minusTax = 0.05;
AngleState angleState;

void AlterFunc_TextDrawUTF8_1(Text*t, TextChar*c)
{
    if(c->chr_textIdx == 0){
        radius = 300;
        angle = 0;
        position = 0;
        cY_min = 0;
    }

    c->x = -radius * cos(DEG_TO_RAD(angle));
    c->y = radius * sin(DEG_TO_RAD(angle));
    c->rotation = DEG_TO_RAD(angle);

    if(cY_min < c->y){
        cY_min = c->y;
    }

    angle += 360.0 / (Text_GetSizeUTF8(t));
    ++position;
    radius += minus;
}

//
void AlterFunc_TextDrawUTF8_2(Text*t, TextChar*c)
{
    if(c->chr_textIdx == 0){
        angle = angle_init;
        cY = cX = 0;
    }

    // c->y = 3 * sin(DEG_TO_RAD(angle)) + cY;
    c->y += 10 * sin(DEG_TO_RAD(c->chr_lineIdx * 5 + angle_init));
    angle += 5;
    angle = fmod(angle, 360);
    // printf("%.2lf\n", angle);
    // printf("%li %li\n", c->chr_lineIdx, c->line_idx);
}

//
double color_init = 0;
void AlterFunc_TextDrawUTF8_3(Text*t, TextChar*c)
{
    if(c->chr_textIdx == 0){
        color_init += 0.05;
        color_init = fmod(color_init, Text_GetSizeUTF8(t));
    }

    //*
    c->color = (SDL_Color){
        .r = 255 * ((c->chr_textIdx + (int)color_init) % 10) / 9,
        .g = 255 * ((c->chr_textIdx + 1 + (int)color_init) % 10) / 9,
        .b = 255 * ((c->chr_textIdx + 2 + (int)color_init) % 10) / 9,
        .a = 255
    };
    // */
    c->color.a = 255;
    // printf("%i %i %i\n", c->color.r, c->color.g, c->color.b);
}

//
double dist;
double dist_init = 0.5, dist_init_tax = 0.005;
double dist_tax;
const double dist_min = 0.5, dist_max = 2;
void AlterFunc_TextDrawUTF8_4(Text*t, TextChar*c)
{
    if(c->chr_textIdx == 0){
        dist = dist_init;
        dist_tax = 0.1;
    }

    c->width *= dist;
    c->height *= dist;
    //*
    if(dist > dist_max || dist < dist_min){
        dist_tax *= -1;
    }
    // */
    dist += dist_tax;
}

//
// Main
//
void Text_Test2(char*fpath, unsigned ptsize, char*str)
{
    Text *t;
    int t_edges[DRAW_EDGE_AMT];
    TextCache *cache;
    //
    double fps;
    double fps_delay;
    unsigned fps_time;
    unsigned fps_delayTime;
    //
    extern double minus;
    //
    SDL_Window *w;
    SDL_Renderer *r;
    SDL_Event e;

    if(graphical_init() != 0){

        exit(1);

    }
    w = SDL_CreateWindow(
        "2nd test of Text struct",
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
    cache = TextCache_Create(r, fpath, ptsize);
    if(!cache){
        exit(1);
    }
    t = Text_Create(cache);
    //
    fps_delay = 100;
    fps_delayTime = fps_time = SDL_GetTicks();
    //
    minus = angle_init = 0;

    while(1){
        SDL_SetRenderDrawColor(r, 100, 100, 255, 255);
        SDL_RenderClear(r);

        if(SDL_GetTicks() - fps_delayTime >= fps_delay){
            printf("FPS: %.2lf\n", fps);
            fps_delayTime = SDL_GetTicks();
        }

        //*
        Text_DrawUTF8(
            t,
            str,
            TextConfig_Create(
                .color = (SDL_Color){155, 155, 0, 255},
                .scale_width = 1 / 2.0,
                .scale_height = 1 / 2.0,
                .wrap = 1024,
                .x = 300,
                .y = 0,
                .func_alterText = &AlterFunc_TextDrawUTF8_1,

                .margin.top = 10,
                .margin.bottom = 10,
                .margin.right = 10,
                .margin.left = 10,
                .margin.color = (SDL_Color){255, 0, 0, 255},

                .bx_width = 0,
                .bx_height = 0,
            ),
            &t_edges
        );
        // */
        // printf("%i %.2lf\n", (*Text_edges(t))[DRAW_EDGE_XMIN], *Text_x(t));
        //*
        Text_DrawUTF8(
            t,
            str,
            TextConfig_Create(
                .color = (SDL_Color){155, 155, 0, 255},
                .scale_width = 1 / 2.0,
                .scale_height = 1 / 2.0,
                .x = t_edges[DRAW_EDGE_XMIN],
                .y = t_edges[DRAW_EDGE_YMAX] + 9,
                .func_alterText = &AlterFunc_TextDrawUTF8_2,
                //
                .margin.color = (SDL_Color){0, 255, 0, 255},
                .margin.right = 50
            ),
            &t_edges
        );
        // */
        //*
        Text_DrawUTF8(
            t,
            str,
            TextConfig_Create(
                .x = t_edges[DRAW_EDGE_XMIN],
                .y = t_edges[DRAW_EDGE_YMAX],
                .func_alterText = &AlterFunc_TextDrawUTF8_3,
                //
                .margin.left = 0,
                .margin.right = 0,
            ),
            NULL
        );
        // */
        //*
        Text_DrawUTF8(
            t,
            // str,
            "It works! Look at this geometry!",
            TextConfig_Create(
                .color = (SDL_Color){155, 155, 0, 255},
                .scale_height = 1 / 2.0,
                .scale_width = 1 / 2.0,
                .x = 600,
                .y = 100,
                .func_alterText = &AlterFunc_TextDrawUTF8_4,
            ),
            NULL
        );
        // */

        while(SDL_PollEvent(&e)){
            switch(e.type){
                case SDL_QUIT:
                    SDL_DestroyWindow(w);
                    SDL_DestroyRenderer(r);
                    SDL_Quit();

                    exit(0);

                default:
                    break;
            }
        }

        SDL_RenderPresent(r);
        SDL_Delay(GTEST_DELAY);
        // SDL_Delay(100);

        fps = 1000.0 / (SDL_GetTicks() - fps_time);
        fps_time = SDL_GetTicks();
        // minus
        if(angleState == ANGLE_REDUCE){
            minus -= minusTax;
        }
        else if(angleState == ANGLE_INCREASE){
            minus += minusTax;
        }

        if(minus <= - 4.9){
            angleState = ANGLE_INCREASE;
        }
        else if(minus >= 0){
            angleState = ANGLE_REDUCE;
        }
        // angle_init
        angle_init += 4;
        angle_init = fmod(angle_init, 360);
        // dist_tax
        dist_init += dist_init_tax;
        if(dist_init >= dist_max || dist_init <= dist_min){
            dist_init_tax *= -1;
        }
    }
}

#endif
