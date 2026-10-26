#include"text.h"

/*
 * Wrote with help of Claude
 */

#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define TI_MIN(a, b) (((a) < (b))? (a): (b))
#define TI_MAX(a, b) (((a) > (b))? (a): (b))

#define TI_BLINK_PERIOD_MS  1400
#define TI_BLINK_ON_MS      700
#define TI_TAB_WIDTH        4
#define TI_MAX_CODEPOINTS   (128 * 1024)

static char TI_buffer[1024 * 512]; // 0.5 Mbi, only used by TextInput_format
static TextConfig *TI_buffer_tc;
//

// A place where the caret can be drawn. Positions at a soft-wrap boundary exist twice
// (end of the upper line, bol = 0, and start of the lower line, bol = 1).
typedef struct TI_Caret TI_Caret;
struct TI_Caret {
    size_t idx;
    int bol;
    int x, y;
};

/* ------------------------------------------------------------------ */
/* Text model: the raw UTF-8 string decoded into code points           */
/* ------------------------------------------------------------------ */

static void TI_reserve(TextInput *ti, size_t n)
{
    int *tmp;
    size_t cap;

    if(n <= ti->cap){
        return;
    }
    cap = (ti->cap)? ti->cap: 64;
    while(cap < n){
        cap *= 2;
    }
    tmp = realloc(ti->cps, cap * sizeof(*ti->cps));
    if(!tmp){
        printf("Error while allocating memory for TextInput\n");
        exit(1);
    }
    ti->cps = tmp;
    ti->cap = cap;
}

// Re-reads the Text raw string. Cheap enough to call at the start of every public
// function, which keeps the input correct even if the text was changed from outside.
static void TI_refresh(TextInput *ti)
{
    const char *p;
    size_t n, old_len;
    int c, changed;

    old_len = ti->len;
    changed = 0;
    n = 0;
    p = *Text_raw(ti->text);
    while(p){
        p = utf8codepoint(p, &c);
        if(c == '\0'){
            break;
        }
        TI_reserve(ti, n + 1);
        if(n >= old_len || ti->cps[n] != c){
            changed = 1;
        }
        ti->cps[n++] = c;
    }
    if(n != old_len){
        changed = 1;
    }
    ti->len = n;
    if(changed){
        ++ti->gen;
    }
    ti->cursor_idx = TI_MIN(ti->cursor_idx, n);
    ti->anchor_idx = TI_MIN(ti->anchor_idx, n);
}

static int TI_selection(const TextInput *ti, size_t *a, size_t *b)
{
    *a = TI_MIN(ti->anchor_idx, ti->cursor_idx);
    *b = TI_MAX(ti->anchor_idx, ti->cursor_idx);
    return *a != *b;
}

static size_t TI_encodedSize(const int *cps, size_t a, size_t b)
{
    size_t size;

    size = 0;
    for(; a < b; ++a){
        size += utf8codepointsize(cps[a]);
    }
    return size;
}

// Writes cps[a, b) at w without a terminator. Returns the new write position.
static char* TI_put(char *w, char *end, const int *cps, size_t a, size_t b)
{
    for(; a < b && w; ++a){
        w = utf8catcodepoint(w, cps[a], (size_t)(end - w));
    }
    return w;
}

// malloc'd, NUL terminated copy of cps[a, b)
static char* TI_encode(const int *cps, size_t a, size_t b)
{
    char *buf, *end;
    size_t size;

    size = TI_encodedSize(cps, a, b) + 1;
    buf = calloc(size, 1);
    if(!buf){
        return NULL;
    }
    end = buf + size - 1;
    if(!TI_put(buf, end, cps, a, b)){
        free(buf);
        return NULL;
    }
    return buf;
}

// Decodes text that is going to be inserted: CRLF/CR become '\n', tabs become spaces
// and other control characters are dropped. Returns the number of code points.
static size_t TI_decodeInsert(const char *s, int *out, size_t max)
{
    const char *p;
    size_t n, k;
    int c;

    n = 0;
    p = s;
    while(*p && n < max){
        p = utf8codepoint(p, &c);
        if(c == '\0'){
            break;
        }
        if(c == '\r'){
            if(*p == '\n'){
                continue;
            }
            c = '\n';
        }
        if(c == '\t'){
            for(k=0; k < TI_TAB_WIDTH && n < max; ++k){
                out[n++] = ' ';
            }
            continue;
        }
        if((c < 0x20 && c != '\n') || c == 0x7F){
            continue;
        }
        out[n++] = c;
    }
    return n;
}

/* ------------------------------------------------------------------ */
/* Words                                                               */
/* ------------------------------------------------------------------ */

static int TI_isBlank(int c)
{
    return c == ' ' || c == '\t';
}

// 0 = whitespace, 1 = word (letters, digits, '_' and any non-ASCII letter), 2 = punctuation
static int TI_charClass(int c)
{
    if(c == ' ' || c == '\t' || c == '\n' || c == '\r'){
        return 0;
    }
    if(
        (c >= '0' && c <= '9') || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')
        || c == '_' || c >= 128
    ){
        return 1;
    }
    return 2;
}

// Start of the word on the left of idx. A line break counts as a boundary of its own.
static size_t TI_wordStart(const TextInput *ti, size_t idx)
{
    int cls;

    if(idx == 0){
        return 0;
    }
    if(ti->cps[idx - 1] == '\n'){
        return idx - 1;
    }
    while(idx > 0 && TI_isBlank(ti->cps[idx - 1])){
        --idx;
    }
    if(idx > 0 && ti->cps[idx - 1] != '\n'){
        cls = TI_charClass(ti->cps[idx - 1]);
        while(idx > 0 && ti->cps[idx - 1] != '\n' && TI_charClass(ti->cps[idx - 1]) == cls){
            --idx;
        }
    }
    return idx;
}

// Start of the next word on the right of idx (like ctrl+right on Windows)
static size_t TI_wordEnd(const TextInput *ti, size_t idx)
{
    int cls;

    if(idx >= ti->len){
        return ti->len;
    }
    if(ti->cps[idx] == '\n'){
        return idx + 1;
    }
    if(!TI_isBlank(ti->cps[idx])){
        cls = TI_charClass(ti->cps[idx]);
        while(idx < ti->len && ti->cps[idx] != '\n' && TI_charClass(ti->cps[idx]) == cls){
            ++idx;
        }
    }
    while(idx < ti->len && TI_isBlank(ti->cps[idx])){
        ++idx;
    }
    return idx;
}

// The run of same-class characters under idx (double click)
static void TI_wordBoundsAt(const TextInput *ti, size_t idx, size_t *a, size_t *b)
{
    int cls;

    *a = *b = 0;
    if(!ti->len){
        return;
    }
    idx = TI_MIN(idx, ti->len - 1);
    // The caret is between characters: prefer the word that ends right before it
    if(idx > 0 && TI_charClass(ti->cps[idx]) == 0 && TI_charClass(ti->cps[idx - 1]) != 0 && ti->cps[idx - 1] != '\n'){
        --idx;
    }
    if(ti->cps[idx] == '\n'){
        *a = idx;
        *b = idx;
        return;
    }
    cls = TI_charClass(ti->cps[idx]);
    *a = idx;
    while(*a > 0 && ti->cps[*a - 1] != '\n' && TI_charClass(ti->cps[*a - 1]) == cls){
        --*a;
    }
    *b = idx + 1;
    while(*b < ti->len && ti->cps[*b] != '\n' && TI_charClass(ti->cps[*b]) == cls){
        ++*b;
    }
}

// The whole line between two '\n' (triple click)
static void TI_lineBoundsAt(const TextInput *ti, size_t idx, size_t *a, size_t *b)
{
    *a = idx = TI_MIN(idx, ti->len);
    while(*a > 0 && ti->cps[*a - 1] != '\n'){
        --*a;
    }
    *b = idx;
    while(*b < ti->len && ti->cps[*b] != '\n'){
        ++*b;
    }
}

/* ------------------------------------------------------------------ */
/* Layout: where is the caret on screen?                               */
/* ------------------------------------------------------------------ */

// chrs_dest has one rect per code point and is rebuilt by Text_DrawUTF8, so it is only
// valid when the text has not changed since the last TextInput_Display.
static int TI_layoutValid(TextInput *ti)
{
    return ti->len > 0
        && *Text_chrsDest(ti->text) != NULL
        && ti->laid_gen == ti->gen
        && ti->laid_len == ti->len;
}

// True when the characters idx - 1 and idx are split by automatic wrapping (not by '\n')
static int TI_isWrapBoundary(const TextInput *ti, const TextCharRect *r, size_t idx)
{
    if(idx == 0 || idx >= ti->len){
        return 0;
    }
    if(ti->cps[idx] == '\n' || ti->cps[idx - 1] == '\n'){
        return 0;
    }
    return (int)r[idx].y != (int)r[idx - 1].y;
}

static void TI_origin(TextInput *ti, int *ox, int *oy)
{
    *ox = (int)(
        *Text_x(ti->text) + *Text_marginLeft(ti->text)
        + *Text_borderLeft(ti->text) + *Text_paddingLeft(ti->text)
    );
    *oy = (int)(
        *Text_y(ti->text) + *Text_marginTop(ti->text)
        + *Text_borderTop(ti->text) + *Text_paddingTop(ti->text)
    );
}

// Top-left pixel of the caret for (idx, bol). Display and navigation both use this
// function, so the caret always moves to where it will actually be drawn.
static void TI_caretPos(TextInput *ti, size_t idx, int bol, int *ox, int *oy)
{
    TextCharRect *r;
    int at_line_start;

    if(!TI_layoutValid(ti)){
        TI_origin(ti, ox, oy);
        return;
    }
    r = *Text_chrsDest(ti->text);
    idx = TI_MIN(idx, ti->len);
    //
    // Drawn on the left edge of the character at idx: first character of the text,
    // first character after a '\n' or (bol) first character of a wrapped line.
    at_line_start =
        idx < ti->len && ti->cps[idx] != '\n'
        && (idx == 0 || ti->cps[idx - 1] == '\n' || (bol && TI_isWrapBoundary(ti, r, idx)));
    if(at_line_start){
        *ox = (int)r[idx].x;
        *oy = (int)r[idx].y;
        return;
    }
    if(idx == 0){
        TI_origin(ti, ox, oy);
        return;
    }
    // Otherwise on the right edge of the previous character
    *ox = (int)(r[idx - 1].x + r[idx - 1].width) - 1;
    *oy = (int)r[idx - 1].y;
}

// malloc'd list of every caret position of the text (NULL without a valid layout)
static TI_Caret* TI_buildCarets(TextInput *ti, size_t *out_n)
{
    TI_Caret *cs;
    TextCharRect *r;
    size_t i, n;

    *out_n = 0;
    if(!TI_layoutValid(ti)){
        return NULL;
    }
    r = *Text_chrsDest(ti->text);
    cs = malloc(2 * (ti->len + 1) * sizeof(*cs));
    if(!cs){
        return NULL;
    }
    n = 0;
    for(i=0; i <= ti->len; ++i){
        cs[n].idx = i;
        cs[n].bol = 0;
        TI_caretPos(ti, i, 0, &cs[n].x, &cs[n].y);
        ++n;
        if(TI_isWrapBoundary(ti, r, i)){
            cs[n].idx = i;
            cs[n].bol = 1;
            TI_caretPos(ti, i, 1, &cs[n].x, &cs[n].y);
            ++n;
        }
    }
    *out_n = n;
    return cs;
}

// Caret closest to a point: first pick the line, then the nearest column in it
static int TI_hitTest(TextInput *ti, int x, int y, size_t *idx, int *bol)
{
    TI_Caret *cs;
    size_t n, i, best;
    int line_y, have, d, best_d;

    cs = TI_buildCarets(ti, &n);
    if(!cs){
        return 0;
    }
    have = 0;
    line_y = cs[0].y;
    for(i=0; i < n; ++i){
        if(cs[i].y <= y && (!have || cs[i].y > line_y)){
            line_y = cs[i].y;
            have = 1;
        }
    }
    if(!have){
        for(i=0; i < n; ++i){
            line_y = TI_MIN(line_y, cs[i].y);
        }
    }
    best = n;
    best_d = 0;
    for(i=0; i < n; ++i){
        if(cs[i].y != line_y){
            continue;
        }
        d = abs(cs[i].x - x);
        if(best == n || d < best_d){
            best = i;
            best_d = d;
        }
    }
    *idx = cs[best].idx;
    *bol = cs[best].bol;
    free(cs);
    return 1;
}

/* ------------------------------------------------------------------ */
/* Caret and selection movement                                        */
/* ------------------------------------------------------------------ */

static void TI_setCaret(TextInput *ti, size_t idx, int bol, int extend)
{
    ti->cursor_idx = TI_MIN(idx, ti->len);
    ti->cursor_bol = bol;
    if(!extend){
        ti->anchor_idx = ti->cursor_idx;
    }
    ti->cursor_wantx = -1;
    ti->blink_t0 = SDL_GetTicks();
}

static void TI_moveLeft(TextInput *ti, int extend, int by_word)
{
    size_t a, b, target;

    if(TI_selection(ti, &a, &b) && !extend && !by_word){
        TI_setCaret(ti, a, 0, 0);
        return;
    }
    target = ti->cursor_idx;
    if(by_word){
        target = TI_wordStart(ti, target);
    }
    else if(target){
        --target;
    }
    TI_setCaret(ti, target, 0, extend);
}

static void TI_moveRight(TextInput *ti, int extend, int by_word)
{
    size_t a, b, target;

    if(TI_selection(ti, &a, &b) && !extend && !by_word){
        TI_setCaret(ti, b, 0, 0);
        return;
    }
    target = ti->cursor_idx;
    if(by_word){
        target = TI_wordEnd(ti, target);
    }
    else if(target < ti->len){
        ++target;
    }
    TI_setCaret(ti, target, 0, extend);
}

// dir < 0: up, dir > 0: down. Keeps the column (cursor_wantx) across consecutive moves.
static void TI_moveVertical(TextInput *ti, int dir, int extend)
{
    TI_Caret *cs;
    size_t n, i, best;
    int cx, cy, wantx, have, line_y, d, best_d;

    cs = TI_buildCarets(ti, &n);
    if(!cs){
        return;
    }
    TI_caretPos(ti, ti->cursor_idx, ti->cursor_bol, &cx, &cy);
    wantx = (ti->cursor_wantx >= 0)? ti->cursor_wantx: cx;
    //
    // Nearest line above/below the current one
    have = 0;
    line_y = cy;
    for(i=0; i < n; ++i){
        if(dir < 0? cs[i].y < cy: cs[i].y > cy){
            if(!have || (dir < 0? cs[i].y > line_y: cs[i].y < line_y)){
                line_y = cs[i].y;
                have = 1;
            }
        }
    }
    if(!have){
        // First line: go to the start, last line: go to the end
        free(cs);
        TI_setCaret(ti, (dir < 0)? 0: ti->len, 0, extend);
        return;
    }
    //
    // Column closest to the wanted one
    best = n;
    best_d = 0;
    for(i=0; i < n; ++i){
        if(cs[i].y != line_y){
            continue;
        }
        d = abs(cs[i].x - wantx);
        if(best == n || d < best_d){
            best = i;
            best_d = d;
        }
    }
    TI_setCaret(ti, cs[best].idx, cs[best].bol, extend);
    ti->cursor_wantx = wantx;
    free(cs);
}

// Start/end of the visual line (soft-wrapped lines count as separate lines)
static void TI_moveToLineEdge(TextInput *ti, int to_end, int extend)
{
    TI_Caret *cs;
    size_t n, i, best, idx;
    int cx, cy;

    cs = TI_buildCarets(ti, &n);
    if(!cs){
        // No layout yet: use the hard lines of the text
        idx = ti->cursor_idx;
        if(to_end){
            while(idx < ti->len && ti->cps[idx] != '\n'){
                ++idx;
            }
        }
        else{
            while(idx > 0 && ti->cps[idx - 1] != '\n'){
                --idx;
            }
        }
        TI_setCaret(ti, idx, 0, extend);
        return;
    }
    TI_caretPos(ti, ti->cursor_idx, ti->cursor_bol, &cx, &cy);
    best = n;
    for(i=0; i < n; ++i){
        if(cs[i].y != cy){
            continue;
        }
        if(best == n || (to_end? cs[i].x >= cs[best].x: cs[i].x < cs[best].x)){
            best = i;
        }
    }
    if(best == n){
        best = 0;
    }
    TI_setCaret(ti, cs[best].idx, cs[best].bol, extend);
    free(cs);
}

/* ------------------------------------------------------------------ */
/* Editing                                                             */
/* ------------------------------------------------------------------ */

// Replaces [a, b) with ins and puts the caret after the inserted text
static void TI_replaceRange(TextInput *ti, size_t a, size_t b, const char *ins)
{
    int *ins_cps;
    char *buf, *w, *end;
    size_t nins, room, size, tmp;

    a = TI_MIN(a, ti->len);
    b = TI_MIN(b, ti->len);
    if(a > b){
        tmp = a;
        a = b;
        b = tmp;
    }
    //
    ins_cps = NULL;
    nins = 0;
    room = (ti->len - (b - a) < TI_MAX_CODEPOINTS)? TI_MAX_CODEPOINTS - (ti->len - (b - a)): 0;
    if(ins && *ins && room){
        ins_cps = malloc((TI_MIN(room, strlen(ins) * TI_TAB_WIDTH) + 1) * sizeof(*ins_cps));
        if(!ins_cps){
            return;
        }
        nins = TI_decodeInsert(ins, ins_cps, room);
    }
    if(a == b && !nins){
        free(ins_cps);
        return;
    }
    //
    size = TI_encodedSize(ti->cps, 0, a)
        + TI_encodedSize(ins_cps, 0, nins)
        + TI_encodedSize(ti->cps, b, ti->len)
        + 1;
    buf = calloc(size, 1);
    if(!buf){
        free(ins_cps);
        return;
    }
    end = buf + size - 1;
    w = TI_put(buf, end, ti->cps, 0, a);
    w = TI_put(w, end, ins_cps, 0, nins);
    w = TI_put(w, end, ti->cps, b, ti->len);
    if(w){
        Text_SetUTF8(ti->text, buf);
        // An empty Text is kept as NULL (as the previous version did)
        if(a + nins + (ti->len - b) == 0 && *Text_raw(ti->text)){
            free(*Text_raw(ti->text));
            *Text_raw(ti->text) = NULL;
        }
        TI_refresh(ti);
        TI_setCaret(ti, a + nins, 0, 0);
    }
    free(buf);
    free(ins_cps);
}

void TextInput_addText(TextInput*ti, char*str)
{
    size_t a, b;

    TI_refresh(ti);
    if(!TI_selection(ti, &a, &b)){
        a = b = ti->cursor_idx;
    }
    TI_replaceRange(ti, a, b, str);
}

void TextInput_delText(TextInput*ti)
{
    size_t a, b;

    TI_refresh(ti);
    if(TI_selection(ti, &a, &b)){
        TI_replaceRange(ti, a, b, NULL);
    }
    else if(ti->cursor_idx){
        TI_replaceRange(ti, ti->cursor_idx - 1, ti->cursor_idx, NULL);
    }
}

void TextInput_delTextForward(TextInput*ti)
{
    size_t a, b;

    TI_refresh(ti);
    if(TI_selection(ti, &a, &b)){
        TI_replaceRange(ti, a, b, NULL);
    }
    else if(ti->cursor_idx < ti->len){
        TI_replaceRange(ti, ti->cursor_idx, ti->cursor_idx + 1, NULL);
    }
}

void TextInput_delWordBackward(TextInput*ti)
{
    size_t a, b;

    TI_refresh(ti);
    if(TI_selection(ti, &a, &b)){
        TI_replaceRange(ti, a, b, NULL);
    }
    else{
        TI_replaceRange(ti, TI_wordStart(ti, ti->cursor_idx), ti->cursor_idx, NULL);
    }
}

void TextInput_delWordForward(TextInput*ti)
{
    size_t a, b;

    TI_refresh(ti);
    if(TI_selection(ti, &a, &b)){
        TI_replaceRange(ti, a, b, NULL);
    }
    else{
        TI_replaceRange(ti, ti->cursor_idx, TI_wordEnd(ti, ti->cursor_idx), NULL);
    }
}

/* ------------------------------------------------------------------ */
/* Selection and clipboard                                             */
/* ------------------------------------------------------------------ */

void TextInput_selectAll(TextInput*ti)
{
    TI_refresh(ti);
    ti->anchor_idx = 0;
    TI_setCaret(ti, ti->len, 0, 1);
}

char* TextInput_getSelectedText(TextInput*ti)
{
    size_t a, b;

    TI_refresh(ti);
    if(!TI_selection(ti, &a, &b)){
        return NULL;
    }
    return TI_encode(ti->cps, a, b);
}

void TextInput_copy(TextInput*ti)
{
    char *str;

    str = TextInput_getSelectedText(ti);
    if(str){
        SDL_SetClipboardText(str);
        free(str);
    }
}

void TextInput_cut(TextInput*ti)
{
    size_t a, b;

    TI_refresh(ti);
    if(!TI_selection(ti, &a, &b)){
        return;
    }
    TextInput_copy(ti);
    TI_replaceRange(ti, a, b, NULL);
}

void TextInput_paste(TextInput*ti)
{
    char *str;

    if(!SDL_HasClipboardText()){
        return;
    }
    str = SDL_GetClipboardText();
    if(str){
        TextInput_addText(ti, str);
        SDL_free(str);
    }
}

/* ------------------------------------------------------------------ */
/* Creation, drawing                                                   */
/* ------------------------------------------------------------------ */

TextInput* TextInput_Create(
    TextCache*cache, TextConfig* (*tc)[INP_STATES_MAX],
    int cursor_w, int cursor_h,
    char *str
    )
{
    TextInput *ti;
    SDL_Renderer *renderer;
    SDL_Texture *prev_target;
    Uint8 r, g, b, a;
    size_t i;

    ti = calloc(1, sizeof(TextInput));
    if(!ti){
        printf("Error while allocating memory for TextInput\n");
        exit(1);
    }

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
    ti->cursor_wantx = -1;
    ti->selection_color = (SDL_Color){80, 140, 255, 110};
    //
    renderer = *TextCache_renderer(cache);
    ti->cursor_h = (!cursor_h)? (int)(*Text_lineskip(ti->text) * *Text_scaleHeight(ti->text)): cursor_h;
    ti->cursor_tex = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_RGBA8888,
        SDL_TEXTUREACCESS_TARGET,
        cursor_w,
        ti->cursor_h
    );
    if(!ti->cursor_tex){
        printf("Error while creating texture for cursor: %s\n", SDL_GetError());
        exit(1);
    }
    prev_target = SDL_GetRenderTarget(renderer);
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);
    SDL_SetRenderTarget(renderer, ti->cursor_tex);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);
    SDL_SetRenderTarget(renderer, prev_target);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
    //
    TI_refresh(ti);

    return ti;
}

void TextInput_Destroy(TextInput**ti)
{
    size_t i;

    if(!ti || !*ti){
        return;
    }
    SDL_DestroyTexture((*ti)->cursor_tex);
    Text_Destroy(&(*ti)->text);
    for(i=0; i < INP_STATES_MAX; ++i){
        free((*ti)->state_tc[i]);
    }
    free((*ti)->cps);
    free(*ti);
    *ti = NULL;
}

static void TI_drawSelection(TextInput *ti)
{
    SDL_Renderer *renderer;
    SDL_BlendMode prev_mode;
    TextCharRect *r;
    SDL_Rect rc;
    Uint8 pr, pg, pb, pa;
    size_t a, b, i;
    int y, x0, x1, xmax, ymax;

    if(!ti->state[INP_STATE_SELECTED] || !TI_selection(ti, &a, &b) || !TI_layoutValid(ti)){
        return;
    }
    renderer = *Text_renderer(ti->text);
    r = *Text_chrsDest(ti->text);
    xmax = (int)(*Text_edges(ti->text))[DRAW_EDGE_XMAX];
    ymax = (int)(*Text_edges(ti->text))[DRAW_EDGE_YMAX];
    //
    SDL_GetRenderDrawBlendMode(renderer, &prev_mode);
    SDL_GetRenderDrawColor(renderer, &pr, &pg, &pb, &pa);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(
        renderer,
        ti->selection_color.r, ti->selection_color.g, ti->selection_color.b, ti->selection_color.a
    );
    // One rectangle per visual line. The rect of a '\n' is not used.
    i = a;
    while(i < b){
        if(ti->cps[i] == '\n'){
            ++i;
            continue;
        }
        y = (int)r[i].y;
        x0 = (int)r[i].x;
        x1 = (int)(r[i].x + r[i].width);
        ++i;
        while(i < b && ti->cps[i] != '\n' && (int)r[i].y == y){
            x1 = (int)(r[i].x + r[i].width);
            ++i;
        }
        if(y >= ymax || x0 >= xmax){
            continue;
        }
        rc = (SDL_Rect){x0, y, TI_MIN(x1, xmax) - x0, ti->cursor_h};
        SDL_RenderFillRect(renderer, &rc);
    }
    SDL_SetRenderDrawColor(renderer, pr, pg, pb, pa);
    SDL_SetRenderDrawBlendMode(renderer, prev_mode);
}

static void TI_drawCaret(TextInput *ti)
{
    SDL_Rect dest;

    if(!ti->state[INP_STATE_SELECTED]){
        return;
    }
    // The blink restarts with every user action, so the caret is always visible while typing
    if((SDL_GetTicks() - ti->blink_t0) % TI_BLINK_PERIOD_MS > TI_BLINK_ON_MS){
        return;
    }
    TI_caretPos(ti, ti->cursor_idx, ti->cursor_bol, &ti->cursor_x, &ti->cursor_y);
    if(
        ti->cursor_x >= (*Text_edges(ti->text))[DRAW_EDGE_XMAX]
        || ti->cursor_y >= (*Text_edges(ti->text))[DRAW_EDGE_YMAX]
    ){
        return;
    }
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
}

void TextInput_displayCursor(TextInput*ti)
{
    TI_refresh(ti);
    TI_drawCaret(ti);
}

void TextInput_Display(TextInput*ti, int x, int y, void(*func)(TextInput*))
{
    TextCharRect *chrs_dest;

    TI_buffer_tc = 
        (ti->state[INP_STATE_SELECTED] && ti->state_tc[INP_STATE_SELECTED])? ti->state_tc[INP_STATE_SELECTED]:
        (ti->state[INP_STATE_HOVER] && ti->state_tc[INP_STATE_HOVER])? ti->state_tc[INP_STATE_HOVER]:
        ti->state_tc[INP_STATE_NEUTRAL]
        ;
    if(!TI_buffer_tc){
        return;
    }

    if(func){
        func(ti);
    }
    TI_refresh(ti);

    TI_buffer_tc->x = x;
    TI_buffer_tc->y = y;

    Text_DrawUTF8(
        ti->text,
        NULL,
        TI_buffer_tc,
        NULL
    );
    // From here on chrs_dest describes the current text
    ti->laid_gen = ti->gen;
    ti->laid_len = ti->len;
    //
    chrs_dest = *Text_chrsDest(ti->text);
    TI_buffer_tc->bx_height = 
        (chrs_dest && ti->len)?  chrs_dest[ti->len - 1].y + chrs_dest[ti->len - 1].height - *Text_y(ti->text)
        : *Text_lineskip(ti->text) * *Text_scaleHeight(ti->text);

    TI_drawSelection(ti);
    TI_drawCaret(ti);
}

/* ------------------------------------------------------------------ */
/* Public caret movement (selection is collapsed)                      */
/* ------------------------------------------------------------------ */

void TextInput_moveCursorLeft(TextInput*ti)
{
    TI_refresh(ti);
    TI_moveLeft(ti, 0, 0);
}

void TextInput_moveCursorRight(TextInput*ti)
{
    TI_refresh(ti);
    TI_moveRight(ti, 0, 0);
}

void TextInput_moveCursorUp(TextInput*ti)
{
    TI_refresh(ti);
    TI_moveVertical(ti, -1, 0);
}

void TextInput_moveCursorDown(TextInput*ti)
{
    TI_refresh(ti);
    TI_moveVertical(ti, 1, 0);
}

void TextInput_moveCursorToEndOfLine(TextInput*ti)
{
    TI_refresh(ti);
    TI_moveToLineEdge(ti, 1, 0);
}

void TextInput_moveCursorToInitOfLine(TextInput*ti)
{
    TI_refresh(ti);
    TI_moveToLineEdge(ti, 0, 0);
}


// Returns the text with a '-' where a word was split by automatic wrapping
char* TextInput_format(TextInput*ti)
{
    TextCharRect *r;
    char *w, *end;
    size_t i;
    int use_layout;

    TI_refresh(ti);
    if(!ti->len){
        return NULL;
    }
    use_layout = TI_layoutValid(ti);
    r = (use_layout)? *Text_chrsDest(ti->text): NULL;

    memset(TI_buffer, 0, sizeof(TI_buffer));
    w = TI_buffer;
    end = TI_buffer + sizeof(TI_buffer) - 1;
    for(i=0; i < ti->len && w; ++i){
        if(
            use_layout && TI_isWrapBoundary(ti, r, i)
            && TI_charClass(ti->cps[i]) != 0 && TI_charClass(ti->cps[i - 1]) != 0
        ){
            w = utf8catcodepoint(w, '-', (size_t)(end - w));
        }
        if(w){
            w = utf8catcodepoint(w, ti->cps[i], (size_t)(end - w));
        }
    }

    return TI_buffer;
}

/* ------------------------------------------------------------------ */
/* Events                                                              */
/* ------------------------------------------------------------------ */

static int TI_pointInside(TextInput *ti, int x, int y)
{
    return
        x >= (*Text_edges(ti->text))[DRAW_EDGE_PADDING_XMIN] && x <= (*Text_edges(ti->text))[DRAW_EDGE_PADDING_XMAX]
        &&
        y >= (*Text_edges(ti->text))[DRAW_EDGE_PADDING_YMIN] && y <= (*Text_edges(ti->text))[DRAW_EDGE_PADDING_YMAX]
        ;
}

//
void TextInput_handleMouseButton(TextInput*ti, SDL_Event*e, void(*func)(TextInput*, SDL_Event*))
{
    size_t idx, a, b;
    int bol, shift;

    if(e->type == SDL_MOUSEBUTTONUP){
        if(e->button.button == SDL_BUTTON_LEFT){
            ti->dragging = 0;
        }
        return;
    }
    if(e->type != SDL_MOUSEBUTTONDOWN || e->button.button != SDL_BUTTON_LEFT){
        return;
    }

    TI_refresh(ti);
    ti->state[INP_STATE_SELECTED] = TI_pointInside(ti, e->button.x, e->button.y);
    if(!ti->state[INP_STATE_SELECTED]){
        // Lost focus: drop the selection
        ti->dragging = 0;
        ti->anchor_idx = ti->cursor_idx;
    }
    else if(TI_hitTest(ti, e->button.x, e->button.y, &idx, &bol)){
        shift = (SDL_GetModState() & KMOD_SHIFT) != 0;
        if(e->button.clicks >= 3){
            TI_lineBoundsAt(ti, idx, &a, &b);
            ti->anchor_idx = a;
            TI_setCaret(ti, b, 0, 1);
        }
        else if(e->button.clicks == 2){
            TI_wordBoundsAt(ti, idx, &a, &b);
            ti->anchor_idx = a;
            TI_setCaret(ti, b, 0, 1);
        }
        else{
            TI_setCaret(ti, idx, bol, shift);
        }
        ti->dragging = 1;
    }

    if(func){
        func(ti, e);
    }
}

void TextInput_handleMouseMotion(TextInput*ti, SDL_Event*e, void(*func)(TextInput*, SDL_Event*))
{
    if(e->type != SDL_MOUSEMOTION){
        return;
    }

    int x, y;
    size_t idx;
    int bol;

    x = e->motion.x;
    y = e->motion.y;
    ti->state[INP_STATE_HOVER] = 
        x >= (*Text_edges(ti->text))[DRAW_EDGE_BORDER_XMIN] && x <= (*Text_edges(ti->text))[DRAW_EDGE_BORDER_XMAX]
        && y >= (*Text_edges(ti->text))[DRAW_EDGE_BORDER_YMIN] && y <= (*Text_edges(ti->text))[DRAW_EDGE_BORDER_YMAX]
        ;

    // Drag selection (the button may have been released outside the window)
    if(ti->dragging && !(e->motion.state & SDL_BUTTON_LMASK)){
        ti->dragging = 0;
    }
    if(ti->dragging && ti->state[INP_STATE_SELECTED]){
        TI_refresh(ti);
        if(TI_hitTest(ti, x, y, &idx, &bol)){
            TI_setCaret(ti, idx, bol, 1);
        }
    }

    if(func){
        func(ti, e);
    }
}

void TextInput_handleKeyDown(TextInput*ti, SDL_Event*e, void(*func)(TextInput*, SDL_Event*))
{
    if(e->type != SDL_KEYDOWN){
        return;
    }

    //
    if(ti->state[INP_STATE_SELECTED]){
        const Uint16 mod = e->key.keysym.mod;
        const int shift = (mod & KMOD_SHIFT) != 0;
        // Ctrl (Cmd on macOS). Ctrl+Alt is AltGr on some layouts, so it is ignored.
        const int ctrl = (mod & (KMOD_CTRL | KMOD_GUI)) != 0 && !(mod & KMOD_ALT);

        TI_refresh(ti);
        switch(e->key.keysym.sym){
            // Clipboard and selection
            case SDLK_a:
                if(ctrl){
                    TextInput_selectAll(ti);
                }
                break;

            case SDLK_c:
                if(ctrl){
                    TextInput_copy(ti);
                }
                break;

            case SDLK_x:
                if(ctrl){
                    TextInput_cut(ti);
                }
                break;

            case SDLK_v:
                if(ctrl){
                    TextInput_paste(ti);
                }
                break;

            // Deleting
            case SDLK_BACKSPACE:
                (ctrl)? TextInput_delWordBackward(ti): TextInput_delText(ti);
                break;

            case SDLK_DELETE:
                (ctrl)? TextInput_delWordForward(ti): TextInput_delTextForward(ti);
                break;

            // Control cursor idx (shift extends the selection, ctrl jumps by word)
            case SDLK_LEFT:
                TI_moveLeft(ti, shift, ctrl);
                break;

            case SDLK_RIGHT:
                TI_moveRight(ti, shift, ctrl);
                break;

            case SDLK_UP:
                TI_moveVertical(ti, -1, shift);
                break;

            case SDLK_DOWN:
                TI_moveVertical(ti, 1, shift);
                break;

            case SDLK_HOME:
                if(ctrl){
                    TI_setCaret(ti, 0, 0, shift);
                }
                else{
                    TI_moveToLineEdge(ti, 0, shift);
                }
                break;

            case SDLK_END:
                if(ctrl){
                    TI_setCaret(ti, ti->len, 0, shift);
                }
                else{
                    TI_moveToLineEdge(ti, 1, shift);
                }
                break;

            case SDLK_RETURN:
            case SDLK_KP_ENTER:
                TextInput_addText(ti, "\n");
                break;

            case SDLK_TAB:
                TextInput_addText(ti, "    ");
                break;

            default:
                break;
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
    // Only the focused input receives typed text
    if(!ti->state[INP_STATE_SELECTED]){
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
