#ifndef INP_TEXT_H
#define INP_TEXT_H

/*
 * Wrote with help of Claude
 */

#include"inputs_communs.h"
#include<graphical.h>

typedef struct TextInput TextInput;
struct TextInput {
    int state[INP_STATES_MAX];
    TextConfig* state_tc[INP_STATES_MAX];

    // Caret
    SDL_Texture *cursor_tex;
    int cursor_x, cursor_y;     // last drawn caret position (pixels)
    int cursor_h;               // caret height (pixels)
    size_t cursor_idx;          // caret position, in code points
    int cursor_bol;             // at a soft-wrap boundary: 1 = caret drawn at the start of the lower line
    int cursor_wantx;           // column kept while moving up/down (-1 = none)
    Uint32 blink_t0;            // tick of the last user action (restarts the blink)

    // Selection: the range between anchor_idx and cursor_idx (empty when equal)
    size_t anchor_idx;
    int dragging;               // left mouse button held after a click inside the input
    SDL_Color selection_color;

    // Text as code points, rebuilt from the Text raw string (see TI_refresh)
    int *cps;
    size_t len, cap;

    // Layout tracking: chrs_dest is only trustworthy when laid_gen == gen
    unsigned gen, laid_gen;
    size_t laid_len;

    Text *text;
};

TextInput* TextInput_Create(
    TextCache*, TextConfig* (*)[INP_STATES_MAX],
    int cursor_w, int cursor_h,
    char *str
);
void TextInput_Destroy(TextInput**);
void TextInput_Display(TextInput*, int x, int y, void(*)(TextInput*));
//
void TextInput_displayCursor(TextInput*);

// Editing (every function replaces the selection when there is one)
void TextInput_addText(TextInput*, char*str);
void TextInput_delText(TextInput*);             // backspace
void TextInput_delTextForward(TextInput*);      // delete
void TextInput_delWordBackward(TextInput*);     // ctrl + backspace
void TextInput_delWordForward(TextInput*);      // ctrl + delete

// Selection and clipboard
void TextInput_selectAll(TextInput*);
char* TextInput_getSelectedText(TextInput*);    // malloc'd, free() it; NULL without selection
void TextInput_copy(TextInput*);
void TextInput_cut(TextInput*);
void TextInput_paste(TextInput*);

// Caret movement (these collapse the selection; shift+key in handleKeyDown extends it)
void TextInput_moveCursorLeft(TextInput*ti);
void TextInput_moveCursorRight(TextInput*ti);
void TextInput_moveCursorUp(TextInput*ti);
void TextInput_moveCursorDown(TextInput*ti);

void TextInput_moveCursorToEndOfLine(TextInput*ti);
void TextInput_moveCursorToInitOfLine(TextInput*ti);
//
char* TextInput_format(TextInput*ti);


void TextInput_handleMouseButton(TextInput*, SDL_Event*, void(*)(TextInput*, SDL_Event*));
void TextInput_handleMouseMotion(TextInput*, SDL_Event*, void(*)(TextInput*, SDL_Event*));
void TextInput_handleKeyDown(TextInput*, SDL_Event*, void(*)(TextInput*, SDL_Event*));
void TextInput_handleTextInput(TextInput*, SDL_Event*, void(*)(TextInput*, SDL_Event*));

void TextInput_handleInput(TextInput*, SDL_Event*e);

#endif
