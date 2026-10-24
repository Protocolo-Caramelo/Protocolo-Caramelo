#if defined(COMPILATION_DEVQUOTE) || defined(COMPILATION_TOOLCHAIN_WINDOWS_DEVQUOTE)
#include"GTest.h"

SDL_Window *w;
SDL_Renderer *r;
SDL_Event e;

//
// TextInput / TextBox
//
typedef struct TextInput TextInput;
struct TextInput {
    Text *text;
    TextConfig tc_selected;
    TextConfig tc_unselected;

    int selected;

    SDL_Texture *cursor_tex;
    int cursor_x, cursor_y;
    size_t cursor_idx;
};
#define TEXTINPUT_MAX_TEXT_RAW_BYTE_SIZE 1024
char TI_str[TEXTINPUT_MAX_TEXT_RAW_BYTE_SIZE];
//
TextInput* TextInput_Create(SDL_Renderer*, TextCache*, TextConfig*, TextConfig*, char init_str[]);
void TextInput_Destroy(TextInput**);
void TextInput_Display(TextInput*, int x, int y);

void TextInput_checkSelection(TextInput*, SDL_Event*e);
void TextInput_displayCursor(TextInput*);
void TextInput_handleKeyDown(TextInput*, SDL_Event*e);
void TextInput_handleTextInput(TextInput*, SDL_Event*e, char*str);


TextInput* TextInput_Create(SDL_Renderer *r, TextCache*cache, TextConfig*tc_unselected, TextConfig*tc_selected, char init_str[])
{
    TextInput *n_ti;

    n_ti = malloc(sizeof(TextInput));
    n_ti->text = Text_Create(cache, tc_unselected, init_str);
    n_ti->tc_unselected = *tc_unselected;
    n_ti->tc_selected = *tc_selected;
    //
    n_ti->selected =
        n_ti->cursor_x =
        n_ti->cursor_y =
        n_ti->cursor_idx = 0
        ;
    //
    n_ti->cursor_tex = SDL_CreateTexture(
        r,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        2,
        *Text_lineskip(n_ti->text) * *Text_scaleHeight(n_ti->text)
    );
    if(!n_ti->cursor_tex){
        printf("Error while creating texture for cursor: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_SetRenderTarget(r, n_ti->cursor_tex);
    SDL_SetRenderDrawColor(
        r,
        0,
        0,
        0,
        255
    );
    SDL_RenderClear(r);
    SDL_SetRenderTarget(r, NULL);

    return n_ti;
}

void TextInput_Destroy(TextInput**inp)
{
    SDL_DestroyTexture((*inp)->cursor_tex);
    Text_Destroy(&(*inp)->text);
    free(*inp);
    *inp = NULL;
}

void TextInput_Display(TextInput*ti, int x, int y)
{
    TextConfig *tc;
    TextCharRect *chrs_dest;
    size_t t_size;

    *Text_x(ti->text) = ti->tc_selected.x = ti->tc_unselected.x = x;
    *Text_y(ti->text) = ti->tc_selected.y = ti->tc_unselected.y = y;
    tc = (ti->selected)? &ti->tc_selected: &ti->tc_unselected;
    // printf("%i %i\n", *Text_x(ti->text), *Text_y(ti->text));
    t_size = Text_GetSizeUTF8(ti->text);
    Text_DrawUTF8(
        ti->text,
        NULL,
        tc,
        NULL
    );
    //
    //
    chrs_dest = *Text_chrsDest(ti->text);
    ti->tc_selected.bx_height =
    ti->tc_unselected.bx_height =
        (chrs_dest && t_size)?  chrs_dest[t_size - 1].y + chrs_dest[t_size - 1].height - *Text_y(ti->text)
        : *Text_lineskip(ti->text) * *Text_scaleHeight(ti->text);


    TextInput_displayCursor(ti);
}

void TextInput_checkSelection(TextInput*inp, SDL_Event*e)
{
    if(e->type != SDL_MOUSEBUTTONDOWN){
        return;
    }

    int x, y;
    size_t i;
    TextCharRect *chr_rect;

    x = e->button.x;
    y = e->button.y;
    //
    inp->selected = 
        x >= (*Text_edges(inp->text))[DRAW_EDGE_PADDING_XMIN] && x <= (*Text_edges(inp->text))[DRAW_EDGE_PADDING_XMAX]
        &&
        y >= (*Text_edges(inp->text))[DRAW_EDGE_PADDING_YMIN] && y <= (*Text_edges(inp->text))[DRAW_EDGE_PADDING_YMAX]
        ;

    //
    size_t min_distI;
    size_t min_distY_i, min_distX_i;
    size_t t_size;
    TextCharRect *chrs_dest;

    min_distY_i = min_distX_i = min_distI = 1;
    t_size = Text_GetSizeUTF8(inp->text);
    chrs_dest = *Text_chrsDest(inp->text);
    for(i=0; i < t_size && inp->selected && chrs_dest; ++i){
        if(
            (y >= chrs_dest[i].y && y <= chrs_dest[i].y + chrs_dest[i].height)
            || (y < chrs_dest[i].y)
         ){
            break;
        }
        ++min_distY_i;
    }
    // printf("%li\n", min_distY_i);

    min_distX_i = min_distI = min_distY_i;
    for(i=min_distY_i - 1; i < t_size && inp->selected && chrs_dest; ++i){
        if(
            (x >= chrs_dest[i].x && x <= chrs_dest[i].x + chrs_dest[i].width)
            || x < chrs_dest[i].x
            || chrs_dest[min_distY_i].y < chrs_dest[i].y
        ){
            break;
        }
        ++min_distX_i;
    }
    // printf("%li\n", min_distX_i);

    min_distI = min_distX_i;
    if(x <= chrs_dest[i].x + chrs_dest[i].width / 2 || x > chrs_dest[i].x + chrs_dest[i].width){
        --min_distI;
    }
    // printf("%li\n\n", min_distI);
    inp->cursor_idx = min_distI;
}

void TextInput_displayCursor(TextInput*inp)
{
    if(!inp->selected){
        return;
    }
    if(!*Text_raw(inp->text)){
        inp->cursor_idx = 0;
    }

    TextCharRect *chrs_dest;
    SDL_Rect dest;

    chrs_dest = *Text_chrsDest(inp->text);
    //
    inp->cursor_x = *Text_x(inp->text) + *Text_marginLeft(inp->text) + *Text_borderLeft(inp->text) + *Text_paddingLeft(inp->text);
    inp->cursor_y = *Text_y(inp->text) + *Text_marginTop(inp->text) + *Text_borderTop(inp->text) + *Text_paddingTop(inp->text);
    if(chrs_dest && inp->cursor_idx){
        inp->cursor_x = chrs_dest[inp->cursor_idx - 1].x + chrs_dest[inp->cursor_idx - 1].width - 1;
        inp->cursor_y = chrs_dest[inp->cursor_idx - 1].y;
        // printf("(%i %i) (%.2lf %.2lf)\n", inp->cursor_x, inp->cursor_y, *Text_x(inp->text), *Text_y(inp->text));
    }

    if(
        inp->cursor_x >= (*Text_edges(inp->text))[DRAW_EDGE_XMAX]
        || inp->cursor_y >= (*Text_edges(inp->text))[DRAW_EDGE_YMAX]
    ){
        return;
    }

    // printf("%li\n", inp->cursor_idx);
    dest = (SDL_Rect){
        .x = inp->cursor_x,
        .y = inp->cursor_y
    };
    SDL_QueryTexture(
        inp->cursor_tex,
        NULL, NULL,
        &dest.w, &dest.h
    );
    SDL_RenderCopy(
        r,
        inp->cursor_tex,
        NULL,
        &dest
    );
    // */
}

void TextInput_handleKeyDown(TextInput*inp, SDL_Event*e)
{
    char *byte_raw;
    char *byte_str;
    int c, c_inp;
    size_t i;
    //
    switch(e->key.keysym.sym){
    // if(e->key.keysym.sym == SDLK_BACKSPACE && Text_raw(inp->text)){
        case SDLK_BACKSPACE:
            memset(TI_str, 0, sizeof(TI_str) / sizeof(TI_str[0]));

            byte_str = TI_str;
            i = 0;
            while(
                i < Text_GetSizeUTF8(inp->text)
                && (c = Text_getUTF8(inp->text, i))
                ){
                if(inp->cursor_idx && i == inp->cursor_idx - 1){
                    ++i;
                    continue;
                }
                byte_str = utf8catcodepoint(byte_str, c, utf8codepointsize(c));
                // printf("%s\n", TI_str);
                ++i;
            }
            Text_SetUTF8(inp->text, TI_str);
            (inp->cursor_idx)? inp->cursor_idx--: inp->cursor_idx;
            break;
    // Control cursor idx
    // else if(e->key.keysym.sym == SDLK_LEFT && Text_raw(inp->text) && inp->cursor_idx){
        case SDLK_LEFT:
            if(inp->cursor_idx){
                inp->cursor_idx--;
            }
            break;

    // else if(e->key.keysym.sym == SDLK_RIGHT && Text_raw(inp->text) && inp->cursor_idx < Text_GetSizeUTF8(inp->text)){
        case SDLK_RIGHT:
            if(inp->cursor_idx < Text_GetSizeUTF8(inp->text)){
                inp->cursor_idx++;
            }
            break;

        case SDLK_HOME:
            inp->cursor_idx = 0;
            break;

        case SDLK_END:
            inp->cursor_idx = Text_GetSizeUTF8(inp->text);
            break;

        case SDLK_RETURN:
            TextInput_handleTextInput(inp, e, "\n");
            break;

        case SDLK_TAB:
            TextInput_handleTextInput(inp, e, "    ");
            break;

        default:
            break;
    }

    if(inp->cursor_idx == 0 && Text_GetSizeUTF8(inp->text) == 0 && *Text_raw(inp->text)){
        free(*Text_raw(inp->text));
        *Text_raw(inp->text) = NULL;
    }
}

void TextInput_handleTextInput(TextInput*inp, SDL_Event*e, char*str)
{
    char *byte_raw;
    char *byte_str;
    char *byte_inp;
    int c, c_inp;
    size_t i, nw_cursorIdx;
    //
    memset(TI_str, 0, sizeof(TI_str) / sizeof(TI_str[0]));
    c = '\0';
    byte_raw = (*Text_raw(inp->text))? utf8codepoint(*Text_raw(inp->text), &c): NULL;
    byte_str = TI_str;
    byte_inp = utf8codepoint(str, &c_inp);
    i = 0;
    nw_cursorIdx = inp->cursor_idx;
    do{
        if(inp->cursor_idx && c != '\0'){
            byte_str = utf8catcodepoint(byte_str, c, sizeof(TI_str) - (size_t)(byte_str - TI_str));
        }
        if(
           (!inp->cursor_idx && i == inp->cursor_idx)
           || (inp->cursor_idx && i == inp->cursor_idx - 1)
        ){
            while(c_inp != '\0'){
                byte_str = utf8catcodepoint(byte_str, c_inp, sizeof(TI_str) - (size_t)(byte_str - TI_str));
                byte_inp = utf8codepoint(byte_inp, &c_inp);
                ++nw_cursorIdx;
            }
        }
        if(!inp->cursor_idx && c != '\0'){
            byte_str = utf8catcodepoint(byte_str, c, sizeof(TI_str) - (size_t)(byte_str - TI_str));
        }
        // byte_raw = utf8codepoint(byte_raw, &c);
        ++i;
    } while(c != '\0' && (byte_raw = utf8codepoint(byte_raw, &c)));
    inp->cursor_idx = nw_cursorIdx;
    Text_SetUTF8(inp->text, TI_str);
}

void TextInput_checkInput(TextInput*inp, SDL_Event*e)
{
    if(!inp->selected){
        return;
    }

    if(e->type == SDL_KEYDOWN){
        TextInput_handleKeyDown(inp, e);
    }

    if(e->type == SDL_TEXTINPUT){
        TextInput_handleTextInput(inp, e, e->text.text);
    }
}


static int alterText_mouseX, alterText_mouseY;
static double distX, distY, dist;
static double alterText_sin, alterText_cos, alterText_angle;
static int alterText_relativeX, alterText_relativeY;
void TextInput_alterText(Text*t, TextChar*c)
{
    if(c->chr_textIdx){
        distX = distY = dist = 0;
        alterText_sin = alterText_cos = alterText_angle = 0;
    }
    distX = c->x - alterText_mouseX + alterText_relativeX;
    distY = c->y - alterText_mouseY + alterText_relativeY;
    dist = sqrt(distX * distX + distY * distY);

    alterText_sin = distX / dist;
    alterText_cos = distY / dist;
    alterText_angle = atan2(-alterText_sin, alterText_cos);

    c->rotation = 0;
    if(dist == dist){
        c->rotation = alterText_angle;
    }
}

//
// Bar
//
typedef struct BarInput BarInput;
struct BarInput {
    int rg_min, rg_max;
    int w, h;
    int bar_w, bar_h;
    int selected;

    SDL_Color c_init, c_end;
    SDL_Renderer *r;
    Text *tex;
};
char BarInput_buffer[10];
SDL_Color *BarInput_buffer_bi_cInit, *BarInput_buffer_bi_cEnd ;

BarInput* BarInput_Create(SDL_Renderer*, TextCache*, int min, int max, int w, int h, SDL_Color, SDL_Color);
void BarInput_Destroy(BarInput**);
void BarInput_Display(BarInput*, int x, int y);
double BarInput_GetVal(BarInput*);

void BarInput_handleMouseButtonDown(BarInput*, SDL_Event*);
void BarInput_alterBorder(Text*, Texture*);


BarInput* BarInput_Create(
    SDL_Renderer*r, TextCache*cache,
    int min, int max
    , int w, int h,
    SDL_Color c_init, SDL_Color c_end
    )
{
    BarInput *bi;

    bi = malloc(sizeof(BarInput));
    bi->rg_min = min;
    bi->rg_max = max;
    bi->w = bi->bar_w = w;
    bi->h = bi->bar_h = h;
    bi->selected = 0;
    bi->c_init = c_init;
    bi->c_end = c_end;

    size_t i;
    double rate;
    bi->r = r;
    bi->tex = Text_Create(
        cache,
        TextConfig_Create(
            .scale_width = 1 / 3.6,
            .scale_height = 1 / 3.6,
            .margin = (Margin){
                .top = 2,
                .bottom = 2,
                .right = 2,
                .left = 2,
                .color = (SDL_Color){.a=255}
            },
            .border = (Border){
                .left = w,
                .top = h,
            },
            .padding = (Padding){
                .color = (SDL_Color){.r=255, .g=255, .b=255, .a=255}
            },

            .func_alterBorder = &BarInput_alterBorder,
            .bx_width = 50
        ),
        NULL
    );
    return bi;
}

void BarInput_Destroy(BarInput**bi)
{
    Text_Destroy(&(*bi)->tex);
    free(*bi);
    *bi = NULL;
}

void BarInput_Display(BarInput*bi, int x, int y)
{
    BarInput_buffer[0] = '\0';
    sprintf(BarInput_buffer, "%.0lf", BarInput_GetVal(bi));

    BarInput_buffer_bi_cInit = &bi->c_init;
    BarInput_buffer_bi_cEnd = &bi->c_end;
    Text_DrawUTF8(
        bi->tex,
        BarInput_buffer,
        TextConfig_Create(
            .x = x,
            .y = y
        ),
        NULL
    );
}

double BarInput_GetVal(BarInput*bi)
{
    return bi->bar_w / (double)bi->w * (bi->rg_max - bi->rg_min) + bi->rg_min;
}

void BarInput_SetVaL(BarInput*bi, int val)
{
    *Text_borderLeft(bi->tex) = MIN(MAX((val - bi->rg_min) / (double)bi->rg_max * bi->w, 0), bi->w);
    *Text_paddingLeft(bi->tex) = bi->w - *Text_borderLeft(bi->tex);

    *Text_borderTop(bi->tex) = bi->h;
    *Text_paddingTop(bi->tex) = 0;
    if(*Text_borderLeft(bi->tex)){
        *Text_paddingTop(bi->tex) = bi->h;
        *Text_borderTop(bi->tex) = 0;
    }
    bi->bar_w = *Text_borderLeft(bi->tex);
    bi->bar_h = bi->h;
}

void BarInput_handleMouseButtonDown(BarInput*bi, SDL_Event*e)
{
    if(e->type != SDL_MOUSEBUTTONDOWN && e->type != SDL_MOUSEBUTTONUP){
        return;
    }

    int x, y;
    x = e->button.x;
    y = e->button.y;

    bi->selected = 
        x >= (*Text_edges(bi->tex))[DRAW_EDGE_BORDER_XMIN] && x <= (*Text_edges(bi->tex))[DRAW_EDGE_BORDER_XMAX]
        && y >= (*Text_edges(bi->tex))[DRAW_EDGE_BORDER_YMIN] && y <= (*Text_edges(bi->tex))[DRAW_EDGE_BORDER_YMAX]
        ;
    if(e->type == SDL_MOUSEBUTTONUP && bi->selected){
        bi->selected = 0;
    }
    /*
    printf(
        "(%i %i) (%i %i)\n",
        (*Text_edges(bi->tex))[DRAW_EDGE_TEXT_XMIN],
        (*Text_edges(bi->tex))[DRAW_EDGE_TEXT_XMAX],
        (*Text_edges(bi->tex))[DRAW_EDGE_TEXT_YMIN],
        (*Text_edges(bi->tex))[DRAW_EDGE_TEXT_YMAX]
    );
    // */
}

void BarInput_handleMouseMotion(BarInput*bi, SDL_Event*e)
{
    if((e->type != SDL_MOUSEMOTION && e->type != SDL_MOUSEBUTTONDOWN) || !bi->selected){
        return;
    }

    int x, y;
    //
    double t_sin, t_cos;
    int r_x, r_y;
    int r_xBorder, r_xPadding;

    x = (e->type == SDL_MOUSEMOTION)? e->motion.x: e->button.x;
    y = (e->type == SDL_MOUSEMOTION)? e->motion.y: e->button.y;
    //
    t_sin = sin(DEG_TO_RAD(*Text_rotation(bi->tex)));
    t_cos = cos(DEG_TO_RAD(*Text_rotation(bi->tex)));
    //
    r_x = x * t_cos + y * t_sin;
    r_y = y * t_cos + x * t_sin;
    r_xBorder =
        (*Text_edges(bi->tex))[DRAW_EDGE_BORDER_XMIN] * t_cos
        + (*Text_edges(bi->tex))[DRAW_EDGE_BORDER_YMIN] * t_sin;
    r_xPadding = 
        (*Text_edges(bi->tex))[DRAW_EDGE_TEXT_XMIN] * t_cos
        + (*Text_edges(bi->tex))[DRAW_EDGE_TEXT_YMIN] * t_sin;
    if(bi->selected){
        *Text_borderLeft(bi->tex) = MIN(MAX(r_x - r_xBorder, 0), bi->w);
        // *Text_borderLeft(bi->tex) = MIN(MAX(r_xBorder - r_x, 0), bi->w);
        *Text_paddingLeft(bi->tex) = bi->w - *Text_borderLeft(bi->tex);
        *Text_paddingTop(bi->tex) = bi->h;
        *Text_borderTop(bi->tex) = 0;
    }
    // printf("%i (%i %i) (%i %i)\n", r_x, r_xPadding, r_xBorder, MIN(MAX(r_xPadding - r_x, 0), bi->w), MIN(MAX(r_x - r_xBorder, 0), bi->w));
    // printf("%i %i\n", (*Text_edges(bi->tex))[DRAW_EDGE_BORDER_XMIN], (*Text_edges(bi->tex))[DRAW_EDGE_TEXT_XMIN]);
    /*
    printf(
        "(%i %i) (%i %i)\n",
        (*Text_edges(bi->tex))[DRAW_EDGE_TEXT_XMIN],
        (*Text_edges(bi->tex))[DRAW_EDGE_TEXT_XMAX],
        (*Text_edges(bi->tex))[DRAW_EDGE_TEXT_YMIN],
        (*Text_edges(bi->tex))[DRAW_EDGE_TEXT_YMAX]
    );
    // */
    if(bi->selected && !*Text_paddingLeft(bi->tex)){
        *Text_borderTop(bi->tex) = bi->h;
        *Text_paddingTop(bi->tex) = 0;
    }
    bi->bar_w = *Text_borderLeft(bi->tex);
    bi->bar_h = bi->h;
}

void BarInput_alterBorder(Text*t, Texture*tex)
{
    int w, h;
    size_t i;
    double rate;
    
    SDL_QueryTexture(
        *Texture_raw(tex),
        NULL, NULL,
        &w, &h
    );

    SDL_SetRenderDrawColor(
        *Texture_renderer(tex),
        255, 255, 255, 255
    );
    SDL_RenderClear(*Texture_renderer(tex));
    for(i=0; i < *Text_borderLeft(t); ++i){
        rate = (double)(i + 1) / w;
        
        SDL_SetRenderDrawColor(
            *Texture_renderer(tex),
            BarInput_buffer_bi_cInit->r + rate * (BarInput_buffer_bi_cEnd->r - BarInput_buffer_bi_cInit->r),
            BarInput_buffer_bi_cInit->g + rate * (BarInput_buffer_bi_cEnd->g - BarInput_buffer_bi_cInit->g),
            BarInput_buffer_bi_cInit->b + rate * (BarInput_buffer_bi_cEnd->b - BarInput_buffer_bi_cInit->b),
            BarInput_buffer_bi_cInit->a + rate * (BarInput_buffer_bi_cEnd->a - BarInput_buffer_bi_cInit->a)
        );

        SDL_RenderDrawLine(
            *Texture_renderer(tex),
            i,
            0,
            i,
            h
        );
    }
}

//
// Button
//
SDL_Texture *t_marginTex;
SDL_Rect src_buttonMedium_selected = (SDL_Rect){
    .x = 26,
    .y = 541,
    .w = 1100,
    .h = 1017 - 541
};
SDL_Rect src_buttonMedium_unselected = (SDL_Rect){
    .x = 41,
    .y = 21,
    .w = 1100,
    .h = 1017 - 541
};
int t_selected;

//
// Alter margin
//
void alterMargin_1(Text*t, Texture*tex)
{
    SDL_RenderCopy(
        *Text_renderer(t),
        t_marginTex,
        (!t_selected)? &src_buttonMedium_unselected: &src_buttonMedium_selected,
        NULL
    );
}

//
// Gradient square
//
void draw_gradient_square(
    SDL_Point position,
    int w, int h,
    SDL_Color c_init, SDL_Color c_end,
    SDL_Point c_center
    )
{
    size_t i, j;
    int x, y;
    //
    int dist_x, dist_y;
    double dist;
    double dist_max;
    //
    double rate;

    // Get the max distance from the vertices of square to center
    dist_max = 0;
    dist_x = position.x - c_center.x;
    dist_y = position.y - c_center.y;
    if((dist = dist_x * dist_x + dist_y * dist_y) > dist_max){
        dist_max = abs(dist);
    }
    
    dist_x = position.x- c_center.x;
    dist_y = position.y- c_center.y + h;
    if((dist = dist_x * dist_x + dist_y * dist_y) > dist_max){
        dist_max = abs(dist);
    }

    dist_x = position.x - c_center.x + w;
    dist_y = position.y - c_center.y + h;
    if((dist = dist_x * dist_x + dist_y * dist_y) > dist_max){
        dist_max = abs(dist);
    }

    dist_x = position.x - c_center.x + w;
    dist_y = position.y - c_center.y;
    if((dist = dist_x * dist_x + dist_y * dist_y) > dist_max){
        dist_max = abs(dist);
    }

    dist_max = sqrt(dist_max);

    //
    for(i=0; i < h; ++i){
        for(j=0; j < w; ++j){
            x = position.x + j;
            y = position.y + i;
            dist_x = x - c_center.x;
            dist_y = y - c_center.y;

            dist = sqrt(dist_x * dist_x + dist_y * dist_y);
            rate = dist / dist_max;
            SDL_SetRenderDrawColor(
                r,
                c_init.r + rate * (c_end.r - c_init.r),
                c_init.g + rate * (c_end.g - c_init.g),
                c_init.b + rate * (c_end.b - c_init.b),
                c_init.a + rate * (c_end.a - c_init.a)
            );

            SDL_RenderDrawLine(
                r,
                position.x + j,
                position.y + i,
                position.x + w - 1,
                position.y + i
            );
        }
    }
}

//
void Text_Test4(char*fpath, unsigned ptsize, char*str)
{
    extern SDL_Window *w;
    extern SDL_Renderer *r;
    extern SDL_Event e;
    //
    int w_width, w_height;
    //
    TextCache *cache;
    Text *t;
    extern SDL_Texture *t_marginTex;
    int t_edges[DRAW_EDGE_AMT] = {};
    int t_marginWidth, t_marginHeight;
    double t_marginRatio;
    //
    double fps;
    unsigned fps_time;
    unsigned fps_delayTime;
    const unsigned fps_delay = 100;
    Text *fps_t;
    char fps_tStr[20] = "FPS 60.00";
    int fps_edges[DRAW_EDGE_AMT] = {};
    //
    TextInput *input_text;
    TextInput *inp1, *inp2, *inp3;
    TextConfig input_configUnselected, input_configSelected;
    //
    TextConfig caption_config;
    Text *caption_biCInit, *caption_biCEnd;
    Text *caption_r, *caption_g, *caption_b;
    BarInput *bi_cInitR, *bi_cInitG, *bi_cInitB;
    BarInput *bi_cEndR, *bi_cEndG, *bi_cEndB;
    //
    BarInput *bi_rectWidth, *bi_rectHeight;
    //
    int rect_w, rect_h;
    int rect_colorCenterX, rect_colorCenterY;
    
    if(graphical_init() != 0){
    
        exit(1);
    
    }
    //
    w = SDL_CreateWindow(
        "4nd test of Text",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1024,
        600,
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
    t_marginTex = IMG_LoadTexture(r, "./libgraphical/graphical/GTest/GText_Test4_Asset1.jpeg");
    if(t_marginTex == NULL){
        printf("Errow while loading image ./libgraphical/graphical/GTest/GText_Test4_Asset1.jpeg: %s\n", SDL_GetError());
        exit(1);
    }
    //*
    t = Text_Create(
        cache,
        TextConfig_Create(
            .scale_width = 1 / 3.6,
            .scale_height = 1 / 3.6,
            .color = (SDL_Color){.a = 255},
        ),
        str
    );
    SDL_QueryTexture(
        t_marginTex,
        NULL, NULL, 
        &t_marginWidth, &t_marginHeight
    );
    t_marginRatio = t_marginWidth / t_marginHeight;
    //*
    Text_UpdateConfig(
        t,
        TextConfig_Create(

            .func_alterMargin = &alterMargin_1
        )
    );
    t_selected = 0;
    // */
    //*
    fps_delayTime = fps_time = SDL_GetTicks();
    fps_t = Text_Create(
        cache,
        TextConfig_Create(
            .scale_width = 1 / 3.6,
            .scale_height = 1 / 3.6,
            .color = (SDL_Color){.a = 255}
        ),
        fps_tStr
    );
    // */
    input_configUnselected = input_configSelected = *TextConfig_Create(
        .scale_width = 1 / 3.6,
        .scale_height = 1 / 3.6,
        .color = (SDL_Color){.r = 255, .a = 255},
        .margin = {
            .top = 2,
            .bottom = 2,
            .right = 2,
            .left = 2,
            .color = (SDL_Color){.a=255}
        },
        .padding = {
            .top = 5,
            .bottom = 5,
            .right = 5,
            .left = 5,
            .color = (SDL_Color){.r=255, .g=255, .b=255, .a=255}
        },
        .bx_width = 200,
        .bx_height = TTF_FontLineSkip(*TextCache_font(cache)) / 3.6,
        .wrap = 200,
    );
    input_configSelected.margin = (TextSpacing){
        .top = 5,
        .bottom = 5,
        .right = 5,
        .left = 5,
        .color = (SDL_Color){.a=255}
    };
    input_text = TextInput_Create(r, cache, &input_configUnselected, &input_configSelected, "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^");
    inp1 = TextInput_Create(r, cache, &input_configUnselected, &input_configSelected, "//////////////////////////////////////////////////////");
    inp2 = TextInput_Create(r, cache, &input_configUnselected, &input_configSelected, ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>");
    inp3 = TextInput_Create(r, cache, &input_configUnselected, &input_configSelected, "<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<");
    //
    rect_w = 300;
    rect_h = 300;
    rect_colorCenterX = rect_colorCenterY = 0;
    //
    bi_cInitR = BarInput_Create(
        r,
        cache,
        0, 255,
        150, 10,
        (SDL_Color){.r=255, .g=255, .b=255, .a=255},
        (SDL_Color){.r=255, .g=0, .b=0, .a=255}
    );
    bi_cInitG = BarInput_Create(
        r,
        cache,
        0, 255,
        150, 10,
        (SDL_Color){.r=255, .g=255, .b=255, .a=255},
        (SDL_Color){.r=0, .g=255, .b=0, .a=255}
    );
    bi_cInitB = BarInput_Create(
        r,
        cache,
        0, 255,
        150, 10,
        (SDL_Color){.r=255, .g=255, .b=255, .a=255},
        (SDL_Color){.r=0, .g=0, .b=255, .a=255}
    );
    BarInput_SetVaL(bi_cInitR, 0);
    BarInput_SetVaL(bi_cInitG, 0);
    BarInput_SetVaL(bi_cInitB, 0);

    bi_cEndR = BarInput_Create(
        r,
        cache,
        0, 255,
        150, 10,
        (SDL_Color){.r=255, .g=255, .b=255, .a=255},
        (SDL_Color){.r=255, .g=0, .b=0, .a=255}
    );
    bi_cEndG = BarInput_Create(
        r,
        cache,
        0, 255,
        150, 10,
        (SDL_Color){.r=255, .g=255, .b=255, .a=255},
        (SDL_Color){.r=0, .g=255, .b=0, .a=255}
    );
    bi_cEndB = BarInput_Create(
        r,
        cache,
        0, 255,
        150, 10,
        (SDL_Color){.r=255, .g=255, .b=255, .a=255},
        (SDL_Color){.r=0, .g=0, .b=255, .a=255}
    );
        
    caption_config = *TextConfig_Create(
        .scale_width = 1 / 3.6,
        .scale_height = 1 / 3.6,
        .chr_fmt = CHAR_FMT_UNDERLINE | CHAR_FMT_BOLD,
    );
    caption_biCInit = Text_Create(
        cache,
        &caption_config,
        "Gradient\ncenter color"
    );
    caption_biCEnd = Text_Create(
        cache,
        &caption_config,
        "Gradient\nedge color"
    );
    caption_r = Text_Create(
        cache,
        &caption_config,
        "R"
    );
    caption_g = Text_Create(
        cache,
        &caption_config,
        "G"
    );
    caption_b = Text_Create(
        cache,
        &caption_config,
        "B"
    );
    //
    bi_rectWidth = BarInput_Create(
        r,
        cache,
        0, rect_w,
        rect_w - 55, 5,
        (SDL_Color){.r=122,.g=122,.b=122,.a=255},
        (SDL_Color){.r=122,.g=122,.b=122,.a=255}
    );
    bi_rectHeight = BarInput_Create(
        r,
        cache,
        0, rect_h,
        rect_h - 50, 5,
        (SDL_Color){.r=122,.g=122,.b=122,.a=255},
        (SDL_Color){.r=122,.g=122,.b=122,.a=255}
    );
    *Text_rotation(bi_rectHeight->tex) = 90;


    while(1){
        if(SDL_GetTicks() - fps_delayTime >= fps_delay){
            fps_delayTime = SDL_GetTicks();
            sprintf(fps_tStr, "FPS %.2lf", fps);
        }

        while(SDL_PollEvent(&e)){
            //*
            // TextInput
            TextInput_checkSelection(input_text, &e);
            TextInput_checkSelection(inp1, &e);
            TextInput_checkSelection(inp2, &e);
            TextInput_checkSelection(inp3, &e);

            TextInput_checkInput(input_text, &e);
            TextInput_checkInput(inp1, &e);
            TextInput_checkInput(inp2, &e);
            TextInput_checkInput(inp3, &e);
            // BarInput
            BarInput_handleMouseButtonDown(bi_cInitR, &e);
            BarInput_handleMouseButtonDown(bi_cInitG, &e);
            BarInput_handleMouseButtonDown(bi_cInitB, &e);
            BarInput_handleMouseButtonDown(bi_cEndR, &e);
            BarInput_handleMouseButtonDown(bi_cEndG, &e);
            BarInput_handleMouseButtonDown(bi_cEndB, &e);
            //
            BarInput_handleMouseButtonDown(bi_rectHeight, &e);
            BarInput_handleMouseButtonDown(bi_rectWidth, &e);

            BarInput_handleMouseMotion(bi_cInitR, &e);
            BarInput_handleMouseMotion(bi_cInitG, &e);
            BarInput_handleMouseMotion(bi_cInitB, &e);
            BarInput_handleMouseMotion(bi_cEndR, &e);
            BarInput_handleMouseMotion(bi_cEndG, &e);
            BarInput_handleMouseMotion(bi_cEndB, &e);
            //
            BarInput_handleMouseMotion(bi_rectHeight, &e);
            BarInput_handleMouseMotion(bi_rectWidth, &e);
            // */

            //
            switch(e.type){
                case SDL_QUIT:
                    SDL_DestroyTexture(t_marginTex);
                    //
                    Text_Destroy(&t);
                    Text_Destroy(&fps_t);
                    Text_Destroy(&caption_biCInit);
                    Text_Destroy(&caption_biCEnd);
                    TextCache_Destroy(&cache);
                    //
                    TextInput_Destroy(&input_text);
                    TextInput_Destroy(&inp1);
                    TextInput_Destroy(&inp2);
                    TextInput_Destroy(&inp3);
                    //
                    BarInput_Destroy(&bi_cInitR);
                    BarInput_Destroy(&bi_cInitG);
                    BarInput_Destroy(&bi_cInitB);
                    BarInput_Destroy(&bi_cEndR);
                    BarInput_Destroy(&bi_cEndG);
                    BarInput_Destroy(&bi_cEndB);
                    //
                    BarInput_Destroy(&bi_rectWidth);
                    //
                    SDL_DestroyRenderer(r);
                    SDL_DestroyWindow(w);
                    SDL_Quit();

                    exit(0);
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    int x, y;
                    x = e.button.x;
                    y = e.button.y;
                    if(
                        x >= t_edges[DRAW_EDGE_XMIN] && x <= t_edges[DRAW_EDGE_XMAX]
                        &&
                        y >= t_edges[DRAW_EDGE_YMIN] && y <= t_edges[DRAW_EDGE_YMAX]
                    ){
                        t_selected = !t_selected;
                    }
                    alterText_mouseX = rect_colorCenterX = e.button.x;
                    alterText_mouseY = rect_colorCenterY = e.button.y;
                    //

                    break;

                case SDL_MOUSEMOTION:
                    alterText_mouseX = rect_colorCenterX = e.motion.x;
                    alterText_mouseY = rect_colorCenterY = e.motion.y;
                    break;

                default:
                    break;
            }
        }


        SDL_SetRenderDrawColor(r, 255, 255, 255, 255);
        SDL_RenderClear(r);
        SDL_GetWindowSize(w, &w_width, &w_height);
        // printf("%i\n", (*Text_edgesBack(bi_rectHeight->tex))[DRAW_EDGE_TEXT_XMAX]);
        //* FPS
        Text_DrawUTF8(
            fps_t,
            fps_tStr,
            TextConfig_Create(
                .x = w_width - fps_edges[DRAW_TEXT_WIDTH],
                .y = 0,
            ),
            &fps_edges
        );
        // */
        //* Input boxes
        if(!t_selected){
            input_text->tc_selected.func_alterText = 
                input_text->tc_unselected.func_alterText = 

                inp1->tc_selected.func_alterText = 
                inp1->tc_unselected.func_alterText =

                inp2->tc_selected.func_alterText = 
                inp2->tc_unselected.func_alterText = 

                inp3->tc_selected.func_alterText = 
                inp3->tc_unselected.func_alterText = NULL;
            alterText_mouseX = alterText_mouseY = 0;
        }
        else {
            input_text->tc_selected.func_alterText = 
                input_text->tc_unselected.func_alterText = 

                inp1->tc_selected.func_alterText = 
                inp1->tc_unselected.func_alterText =

                inp2->tc_selected.func_alterText = 
                inp2->tc_unselected.func_alterText = 

                inp3->tc_selected.func_alterText = 
                inp3->tc_unselected.func_alterText = TextInput_alterText;
        }

        alterText_relativeX = *Text_x(input_text->text);
        alterText_relativeY = *Text_y(input_text->text);
        TextInput_Display(input_text, 0, 0);
        //
        alterText_relativeX = *Text_x(inp1->text);
        alterText_relativeY = *Text_y(inp1->text);
        TextInput_Display(inp1, 0, (*Text_edges(input_text->text))[DRAW_EDGE_YMAX]);
        //
        alterText_relativeX = *Text_x(inp2->text);
        alterText_relativeY = *Text_y(inp2->text);
        TextInput_Display(inp2, MAX((*Text_edges(input_text->text))[DRAW_EDGE_XMAX], (*Text_edges(inp1->text))[DRAW_EDGE_XMAX]), 0);
        //
        alterText_relativeX = *Text_x(inp3->text);
        alterText_relativeY = *Text_y(inp3->text);
        TextInput_Display(inp3, MAX((*Text_edges(input_text->text))[DRAW_EDGE_XMAX], (*Text_edges(inp1->text))[DRAW_EDGE_XMAX]), (*Text_edges(inp2->text))[DRAW_EDGE_YMAX]);
        // */
        //* Gradient square
        draw_gradient_square(
            (SDL_Point){
                .x = (w_width - rect_w),
                .y = fps_edges[DRAW_EDGE_TEXT_YMAX]
            },
            BarInput_GetVal(bi_rectWidth),
            BarInput_GetVal(bi_rectHeight),
            (SDL_Color){
                .r=BarInput_GetVal(bi_cInitR),
                .g=BarInput_GetVal(bi_cInitG),
                .b=BarInput_GetVal(bi_cInitB),
                .a=255
            },
            (SDL_Color){
                .r=BarInput_GetVal(bi_cEndR),
                .g=BarInput_GetVal(bi_cEndG),
                .b=BarInput_GetVal(bi_cEndB),
                .a=255
            },
            (SDL_Point){
                .x = rect_colorCenterX,
                .y = rect_colorCenterY
            }
        );
        // */
        //* Button
        Text_DrawUTF8(
            t,
            NULL,
            TextConfig_Create(
                .x = w_width / 2 - t_edges[DRAW_WIDTH] / 2 - 2 * t_selected,
                .y = w_height / 2 - t_edges[DRAW_HEIGHT] / 2 - 2 * t_selected,

                .chr_fmt = (!t_selected)? CHAR_FMT_BOLD: CHAR_FMT_BOLD | CHAR_FMT_UNDERLINE,

                .margin = {
                    .top = t_marginRatio * (t_edges[DRAW_TEXT_HEIGHT] + 40) / 2,
                    .bottom = t_marginRatio * (t_edges[DRAW_TEXT_HEIGHT] + 40) / 2,
                    .right = 40,
                    .left = 40,
                    .color = (SDL_Color){255, 0, 255, 255},
                },
            ),
            &t_edges
        );
        // */
        //* Bar Rect Dimensios Inputs
        bi_rectHeight->c_init = bi_rectWidth->c_init = (SDL_Color){
            .r = BarInput_GetVal(bi_cInitR),
            .g = BarInput_GetVal(bi_cInitG),
            .b = BarInput_GetVal(bi_cInitB),
            .a = 255
        };
        bi_rectHeight->c_end = bi_rectWidth->c_end = (SDL_Color){
            .r = BarInput_GetVal(bi_cEndR),
            .g = BarInput_GetVal(bi_cEndG),
            .b = BarInput_GetVal(bi_cEndB),
            .a = 255
        };
        // */
        BarInput_Display(
            bi_rectHeight,
            w_width - rect_w - rect_w / 1.75,
            fps_edges[DRAW_EDGE_TEXT_YMAX] + rect_h / 2 - 20
        );
        BarInput_Display(
            bi_rectWidth,
            (*Text_edges(bi_rectHeight->tex))[DRAW_EDGE_XMAX],
            (*Text_edges(bi_rectHeight->tex))[DRAW_EDGE_YMAX]
        );
        //* Bar Color Inputs
        Text_DrawUTF8(
            caption_biCInit,
            NULL,
            TextConfig_Create(
                .x = (w_width - bi_cInitR->w - 60)  + bi_cInitR->w / 2 - (*Text_edges(caption_biCInit))[DRAW_WIDTH] / 2 + 15,
                // .y = 330,
                .y = (*Text_edges(bi_rectWidth->tex))[DRAW_EDGE_YMAX] + 15,
                .wrap = 170
            ),
            NULL
        );
        BarInput_Display(bi_cInitR, w_width - bi_cInitR->w - 60, (*Text_edges(caption_biCInit))[DRAW_EDGE_YMAX] + 5);
        BarInput_Display(bi_cInitG, w_width - bi_cInitG->w - 60, (*Text_edges(bi_cInitR->tex))[DRAW_EDGE_YMAX] + 5);
        BarInput_Display(bi_cInitB, w_width - bi_cInitB->w - 60, (*Text_edges(bi_cInitG->tex))[DRAW_EDGE_YMAX] + 5);
        // printf("%i %i\n", (*Text_edges(bi_cInitR->tex))[DRAW_EDGE_XMIN], (*Text_edges(bi_cInitR->tex))[DRAW_EDGE_XMAX]);

        Text_DrawUTF8(
            caption_biCEnd,
            NULL,
            TextConfig_Create(
                .x = ((*Text_edges(bi_cInitR->tex))[DRAW_EDGE_XMIN] - bi_cEndR->w - 60)  + bi_cEndR->w / 2 - (*Text_edges(caption_biCEnd))[DRAW_WIDTH] / 2 + 15,
                // .y = 330,
                .y = (*Text_edges(bi_rectWidth->tex))[DRAW_EDGE_YMAX] + 15,
                .wrap = 170
            ),
            NULL
        );
        BarInput_Display(bi_cEndR, (*Text_edges(bi_cInitR->tex))[DRAW_EDGE_XMIN] - bi_cEndR->w - 60, (*Text_edges(caption_biCEnd))[DRAW_EDGE_YMAX] + 5);
        BarInput_Display(bi_cEndG, (*Text_edges(bi_cInitR->tex))[DRAW_EDGE_XMIN] - bi_cEndG->w - 60, (*Text_edges(bi_cEndR->tex))[DRAW_EDGE_YMAX] + 5);
        BarInput_Display(bi_cEndB, (*Text_edges(bi_cInitR->tex))[DRAW_EDGE_XMIN] - bi_cEndB->w - 60, (*Text_edges(bi_cEndG->tex))[DRAW_EDGE_YMAX] + 5);

        Text_DrawUTF8( // Caption R
            caption_r,
            NULL,
            TextConfig_Create(
                .x = (*Text_edges(bi_cEndR->tex))[DRAW_EDGE_XMIN] - (*Text_edges(caption_r))[DRAW_WIDTH] - 10,
                .y = *Text_y(bi_cEndR->tex) + (*Text_edges(bi_cEndR->tex))[DRAW_HEIGHT] / 2 - (*Text_edges(caption_r))[DRAW_HEIGHT] / 2,
                .color = (SDL_Color){.r=255,.a=255},
            ),
            NULL
        );
        Text_DrawUTF8( // Caption G
            caption_g,
            NULL,
            TextConfig_Create(
                .x = (*Text_edges(bi_cEndG->tex))[DRAW_EDGE_XMIN] - (*Text_edges(caption_r))[DRAW_WIDTH] - 10,
                .y = *Text_y(bi_cEndG->tex) + (*Text_edges(bi_cEndG->tex))[DRAW_HEIGHT] / 2 - (*Text_edges(caption_r))[DRAW_HEIGHT] / 2,
                .color = (SDL_Color){.g=255,.a=255},
            ),
            NULL
        );
        Text_DrawUTF8( // Caption B
            caption_b,
            NULL,
            TextConfig_Create(
                .x = (*Text_edges(bi_cEndB->tex))[DRAW_EDGE_XMIN] - (*Text_edges(caption_r))[DRAW_WIDTH] - 10,
                .y = *Text_y(bi_cEndB->tex) + (*Text_edges(bi_cEndB->tex))[DRAW_HEIGHT] / 2 - (*Text_edges(caption_r))[DRAW_HEIGHT] / 2,
                .color = (SDL_Color){.b=255,.a=255},
            ),
            NULL
        );
        // */


        SDL_RenderPresent(r);
        SDL_Delay(16);
        //
        fps = 1000.0 / (SDL_GetTicks() - fps_time);
        fps_time = SDL_GetTicks();
    }
}

#endif
