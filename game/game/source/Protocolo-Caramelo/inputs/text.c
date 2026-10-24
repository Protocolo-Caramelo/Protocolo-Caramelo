#include"text.h"

static char TI_buffer[1024 * 512]; // 0.5 Mbi
static TextConfig *TI_buffer_tc;
//

TextInput* TextInput_Create(
    TextCache*cache, TextConfig* (*tc)[INP_STATES_MAX],
    int cursor_w, int cursor_h,
    char *str
    )
{
    TextInput *ti;
    size_t i;

    ti = malloc(sizeof(TextInput));

    memset(ti->state, 0, sizeof(ti->state));
    for(i=0; i < INP_STATES_MAX; ++i){
        ti->state_tc[i] = NULL;
        if((*tc)[i]){
            ti->state_tc[i] = malloc(sizeof(*(ti->state_tc[i])));
            *ti->state_tc[i] = *(*tc)[i];
        }
    }
    //
    ti->text = Text_Create(
        cache,
        ti->state_tc[INP_STATE_NEUTRAL],
        str
    );
    //
    ti->cursor_x =
        ti->cursor_y =
        ti->cursor_idx = 0
        ;
    //
    ti->cursor_tex = SDL_CreateTexture(
        *TextCache_renderer(cache),
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        cursor_w,
        (!cursor_h)? *Text_lineskip(ti->text) * *Text_scaleHeight(ti->text): cursor_h
    );
    if(!ti->cursor_tex){
        printf("Error while creating texture for cursor: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_SetRenderTarget(*TextCache_renderer(cache), ti->cursor_tex);
    SDL_SetRenderDrawColor(
        *TextCache_renderer(cache),
        0,
        0,
        0,
        255
    );
    SDL_RenderClear(*TextCache_renderer(cache));
    SDL_SetRenderTarget(*TextCache_renderer(cache), NULL);
    //

    return ti;
}

void TextInput_Destroy(TextInput**ti)
{
    SDL_DestroyTexture((*ti)->cursor_tex);
    Text_Destroy(&(*ti)->text);
    free(*ti);
    *ti = NULL;
}

void TextInput_Display(TextInput*ti, int x, int y, void(*func)(TextInput*))
{
    TextCharRect *chrs_dest;
    size_t t_size;

    TI_buffer_tc = 
        (ti->state[INP_STATE_SELECTED] && ti->state_tc[INP_STATE_SELECTED])? ti->state_tc[INP_STATE_SELECTED]:
        (ti->state[INP_STATE_HOVER] && ti->state_tc[INP_STATE_HOVER])? ti->state_tc[INP_STATE_HOVER]:
        ti->state_tc[INP_STATE_NEUTRAL]
        ;

    if(func){
        func(ti);
    }

    // *Text_x(ti->text) = ti->tc_selected.x = ti->tc_unselected.x = x;
    // *Text_y(ti->text) = ti->tc_selected.y = ti->tc_unselected.y = y;
    TI_buffer_tc->x = x;
    TI_buffer_tc->y = y;
    // printf("%i %i\n", *Text_x(ti->text), *Text_y(ti->text));
    t_size = Text_GetSizeUTF8(ti->text);
    Text_DrawUTF8(
        ti->text,
        NULL,
        TI_buffer_tc,
        NULL
    );
    //
    //
    chrs_dest = *Text_chrsDest(ti->text);
    // ti->tc_selected.bx_height =
    // ti->tc_unselected.bx_height =
    //     (chrs_dest && t_size)?  chrs_dest[t_size - 1].y + chrs_dest[t_size - 1].height - *Text_y(ti->text)
    //     : *Text_lineskip(ti->text) * *Text_scaleHeight(ti->text);
    TI_buffer_tc->bx_height = 
        (chrs_dest && t_size)?  chrs_dest[t_size - 1].y + chrs_dest[t_size - 1].height - *Text_y(ti->text)
        : *Text_lineskip(ti->text) * *Text_scaleHeight(ti->text);


    TextInput_displayCursor(ti);
}

//
void TextInput_displayCursor(TextInput*ti)
{
    if(!ti->state[INP_STATE_SELECTED] || SDL_GetTicks() % 1000 > 500){
        return;
    }
    if(!*Text_raw(ti->text)){
        ti->cursor_idx = 0;
    }

    TextCharRect *chrs_dest;
    SDL_Rect dest;

    chrs_dest = *Text_chrsDest(ti->text);
    //
    ti->cursor_x = *Text_x(ti->text) + *Text_marginLeft(ti->text) + *Text_borderLeft(ti->text) + *Text_paddingLeft(ti->text);
    ti->cursor_y = *Text_y(ti->text) + *Text_marginTop(ti->text) + *Text_borderTop(ti->text) + *Text_paddingTop(ti->text);
    if(chrs_dest && ti->cursor_idx){
        ti->cursor_x = chrs_dest[ti->cursor_idx - 1].x + chrs_dest[ti->cursor_idx - 1].width - 1;
        ti->cursor_y = chrs_dest[ti->cursor_idx - 1].y;
        // printf("(%i %i) (%.2lf %.2lf)\n", ti->cursor_x, ti->cursor_y, *Text_x(ti->text), *Text_y(ti->text));
    }

    if(
        ti->cursor_x >= (*Text_edges(ti->text))[DRAW_EDGE_XMAX]
        || ti->cursor_y >= (*Text_edges(ti->text))[DRAW_EDGE_YMAX]
    ){
        return;
    }

    // printf("%li\n", ti->cursor_idx);
    dest = (SDL_Rect){
        .x = ti->cursor_x,
        .y = ti->cursor_y
    };
    SDL_QueryTexture(
        ti->cursor_tex,
        NULL, NULL,
        &dest.w, &dest.h
    );
    SDL_RenderCopy(
        *Text_renderer(ti->text),
        ti->cursor_tex,
        NULL,
        &dest
    );
    // */
}

void TextInput_addText(TextInput*ti, char*str)
{
    char *byte_raw;
    char *byte_str;
    char *byte_inp;
    int c, c_inp;
    size_t i, nw_cursorIdx;
    //
    // memset(TI_buffer, 0, sizeof(TI_buffer) / sizeof(TI_buffer[0]));
    TI_buffer[0] = '\0';
    c = '\0';
    byte_raw = (*Text_raw(ti->text))? utf8codepoint(*Text_raw(ti->text), &c): NULL;
    byte_str = TI_buffer;
    byte_inp = utf8codepoint(str, &c_inp);
    i = 0;
    nw_cursorIdx = ti->cursor_idx;
    do{
        if(ti->cursor_idx && c != '\0'){
            byte_str = utf8catcodepoint(byte_str, c, sizeof(TI_buffer) - (size_t)(byte_str - TI_buffer));
        }
        if(
           (!ti->cursor_idx && i == ti->cursor_idx)
           || (ti->cursor_idx && i == ti->cursor_idx - 1)
        ){
            while(c_inp != '\0'){
                byte_str = utf8catcodepoint(byte_str, c_inp, sizeof(TI_buffer) - (size_t)(byte_str - TI_buffer));
                byte_inp = utf8codepoint(byte_inp, &c_inp);
                ++nw_cursorIdx;
            }
        }
        if(!ti->cursor_idx && c != '\0'){
            byte_str = utf8catcodepoint(byte_str, c, sizeof(TI_buffer) - (size_t)(byte_str - TI_buffer));
        }
        // byte_raw = utf8codepoint(byte_raw, &c);
        ++i;
    } while(c != '\0' && (byte_raw = utf8codepoint(byte_raw, &c)));
    ti->cursor_idx = nw_cursorIdx;
    Text_SetUTF8(ti->text, TI_buffer);
}

void TextInput_delText(TextInput*ti)
{
    char *byte_str;
    int c;
    size_t i;

    memset(TI_buffer, '\0', Text_GetLenUTF8(ti->text));
    byte_str = TI_buffer;
    i = 0;
    while(
        i < Text_GetSizeUTF8(ti->text)
        && (c = Text_getUTF8(ti->text, i))
        ){
        if(ti->cursor_idx && i == ti->cursor_idx - 1){
            ++i;
            continue;
        }
        byte_str = utf8catcodepoint(byte_str, c, utf8codepointsize(c));
        // printf("%s\n", TI_buffer);
        ++i;
    }
    Text_SetUTF8(ti->text, TI_buffer);
    (ti->cursor_idx)? ti->cursor_idx--: ti->cursor_idx;
}

void TextInput_moveCursorToEndOfLine(TextInput*ti)
{
    int c;
    size_t i;
    size_t t_size;
    int x, y;

    t_size = Text_GetSizeUTF8(ti->text);
    i = (ti->cursor_idx)? ti->cursor_idx + 1: 0;
    x = (*Text_chrsDest(ti->text))[i].x;
    y = (*Text_chrsDest(ti->text))[i].y;
    while(
        i < t_size
        && (c = Text_getUTF8(ti->text, i))
        ){
        if(c == '\n' || c == '\0' || (*Text_chrsDest(ti->text))[i].y != y){
            break;
        }
        ++i;
    }

    ti->cursor_idx = MIN(i, t_size);
}

void TextInput_moveCursorToInitOfLine(TextInput*ti)
{
    int c;
    size_t i;
    size_t t_size;
    int x, y;

    t_size = Text_GetSizeUTF8(ti->text);
    i = (ti->cursor_idx)? ti->cursor_idx - 1: 0;
    x = (*Text_chrsDest(ti->text))[i].x;
    y = (*Text_chrsDest(ti->text))[i].y;
    while(
        i > 1
        && (c = Text_getUTF8(ti->text, i))
        ){
        if(c == '\n' || c == '\0' || (*Text_chrsDest(ti->text))[i].y != y){
            break;
        }
        --i;
    }

    // ti->cursor_idx = (i < t_size)? i + 1: i;
    // ti->cursor_idx = (i)? i - 1: i;
    printf("%li\n", i);
    ti->cursor_idx = MIN((i > 1)? i + 1: 0, t_size);
}

//
void TextInput_handleMouseButton(TextInput*ti, SDL_Event*e, void(*func)(TextInput*, SDL_Event*))
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
    ti->state[INP_STATE_SELECTED] =
        x >= (*Text_edges(ti->text))[DRAW_EDGE_PADDING_XMIN] && x <= (*Text_edges(ti->text))[DRAW_EDGE_PADDING_XMAX]
        &&
        y >= (*Text_edges(ti->text))[DRAW_EDGE_PADDING_YMIN] && y <= (*Text_edges(ti->text))[DRAW_EDGE_PADDING_YMAX]
        ;

    //
    size_t min_distI;
    size_t min_distY_i, min_distX_i;
    size_t t_size;
    TextCharRect *chrs_dest;

    min_distY_i = min_distX_i = min_distI = 1;
    t_size = Text_GetSizeUTF8(ti->text);
    chrs_dest = *Text_chrsDest(ti->text);
    for(i=0; i < t_size && ti->state[INP_STATE_SELECTED] && chrs_dest; ++i){
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
    for(i=min_distY_i - 1; i < t_size && ti->state[INP_STATE_SELECTED] && chrs_dest; ++i){
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
    ti->cursor_idx = min_distI;
}

void TextInput_handleMouseMotion(TextInput*ti, SDL_Event*e, void(*func)(TextInput*, SDL_Event*))
{
    if(e->type != SDL_MOUSEMOTION){
        return;
    }

    int x, y;

    x = e->motion.x;
    y = e->motion.y;
    ti->state[INP_STATE_HOVER] = 
        x >= (*Text_edges(ti->text))[DRAW_EDGE_BORDER_XMIN] && x <= (*Text_edges(ti->text))[DRAW_EDGE_BORDER_XMAX]
        && y >= (*Text_edges(ti->text))[DRAW_EDGE_BORDER_YMIN] && y <= (*Text_edges(ti->text))[DRAW_EDGE_BORDER_YMAX]
        ;
}

void TextInput_handleKeyDown(TextInput*ti, SDL_Event*e, void(*func)(TextInput*, SDL_Event*))
{
    if(e->type != SDL_KEYDOWN){
        return;
    }

    //
    if(ti->state[INP_STATE_SELECTED]){
        switch(e->key.keysym.sym){
            case SDLK_BACKSPACE:
                TextInput_delText(ti);
                break;

            case SDLK_DELETE:
                if(ti->cursor_idx < Text_GetSizeUTF8(ti->text)){
                    ++ti->cursor_idx;
                    TextInput_delText(ti);
                }
                break;
        // Control cursor idx
        // else if(e->key.keysym.sym == SDLK_LEFT && Text_raw(ti->text) && ti->cursor_idx){
            case SDLK_LEFT:
                if(ti->cursor_idx){
                    ti->cursor_idx--;
                }
                break;

        // else if(e->key.keysym.sym == SDLK_RIGHT && Text_raw(ti->text) && ti->cursor_idx < Text_GetSizeUTF8(ti->text)){
            case SDLK_RIGHT:
                if(ti->cursor_idx < Text_GetSizeUTF8(ti->text)){
                    ti->cursor_idx++;
                }
                break;

            case SDLK_HOME:
                // ti->cursor_idx = 0;
                TextInput_moveCursorToInitOfLine(ti);
                break;

            case SDLK_END:
                // ti->cursor_idx = Text_GetSizeUTF8(ti->text);
                TextInput_moveCursorToEndOfLine(ti);
                break;

            case SDLK_RETURN:
                TextInput_addText(ti, "\n");
                break;

            case SDLK_TAB:
                TextInput_addText(ti, "    ");
                break;

            default:
                break;
        }

        if(ti->cursor_idx == 0 && Text_GetSizeUTF8(ti->text) == 0 && *Text_raw(ti->text)){
            free(*Text_raw(ti->text));
            *Text_raw(ti->text) = NULL;
        }
    }

    if(func){
        func(ti, e);
    }
}

void TextInput_handleTextInput(TextInput*ti, SDL_Event*e, void(*func)(TextInput*, SDL_Event*))
{
    if(e->type != SDL_TEXTINPUT){
        return;
    }

    TextInput_addText(ti, e->text.text);
    if(func){
        func(ti, e);
    }
}

void TextInput_handleInput(TextInput*ti, SDL_Event*e)
{
    TextInput_handleMouseButton(ti, e, NULL);
    TextInput_handleMouseMotion(ti, e, NULL);
    TextInput_handleKeyDown(ti, e, NULL);
    TextInput_handleTextInput(ti, e, NULL);
}
