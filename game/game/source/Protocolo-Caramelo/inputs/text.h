#ifndef INP_TEXT_H
#define INP_TEXT_H

#include"inputs_communs.h"
#include<graphical.h>

typedef struct TextInput TextInput;
struct TextInput {
    int state[INP_STATES_MAX];
    TextConfig* state_tc[INP_STATES_MAX];

    SDL_Texture *cursor_tex;
    int cursor_x, cursor_y;
    size_t cursor_idx;

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
void TextInput_addText(TextInput*, char*str);
void TextInput_delText(TextInput*);
void TextInput_moveCursorToEndOfLine(TextInput*ti);
void TextInput_moveCursorToInitOfLine(TextInput*ti);

void TextInput_handleMouseButton(TextInput*, SDL_Event*, void(*)(TextInput*, SDL_Event*));
void TextInput_handleMouseMotion(TextInput*, SDL_Event*, void(*)(TextInput*, SDL_Event*));
void TextInput_handleKeyDown(TextInput*, SDL_Event*, void(*)(TextInput*, SDL_Event*));
void TextInput_handleTextInput(TextInput*, SDL_Event*, void(*)(TextInput*, SDL_Event*));

void TextInput_handleInput(TextInput*, SDL_Event*e);

#endif
