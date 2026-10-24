#ifndef GTEXTCONFIG_H
#define GTEXTCONFIG_H

#include"GTexture.h"
#include"GTextSpacing.h"
#include"GTextTypedefs.h"
#include"GTextChar.h"
#include"GTextCharFormat.h"
#include<limits.h>

#define INVALID_VAL INT_MAX

typedef TextSpacing Padding;
typedef TextSpacing Border;
typedef TextSpacing Margin;

struct TextConfig {
    Padding padding;
    Border border;
    Margin margin;

    SDL_Color color;
    SDL_Color color_bg;
    unsigned lineskip;
    unsigned wrap;
    double rotation;
    SDL_RendererFlip flip;
    // Text box size
    int bx_width;
    int bx_height;
    //
    double scale_width;
    double scale_height;
    //
    int chr_dist;
    double chr_rotation; // deg
    TextCharFormat chr_fmt;
    SDL_RendererFlip chr_flip;
    //
    double x, y;
    
    //
    void(*func_alterMargin)(Text*, Texture*);
    void(*func_alterBorder)(Text*, Texture*);
    void(*func_alterPadding)(Text*, Texture*);
    void(*func_alterText)(Text*, TextChar*);
};

// 
// Methods
//
#define TextConfig_Create(...) (&(TextConfig){ \
    .padding.left = TEXTSPACING_UNINITIALIZED_VAL, \
    .padding.right = TEXTSPACING_UNINITIALIZED_VAL, \
    .padding.top = TEXTSPACING_UNINITIALIZED_VAL, \
    .padding.bottom = TEXTSPACING_UNINITIALIZED_VAL, \
    \
    .border.left = TEXTSPACING_UNINITIALIZED_VAL, \
    .border.right = TEXTSPACING_UNINITIALIZED_VAL, \
    .border.top = TEXTSPACING_UNINITIALIZED_VAL, \
    .border.bottom = TEXTSPACING_UNINITIALIZED_VAL, \
    \
    .margin.left = TEXTSPACING_UNINITIALIZED_VAL, \
    .margin.right = TEXTSPACING_UNINITIALIZED_VAL, \
    .margin.top = TEXTSPACING_UNINITIALIZED_VAL, \
    .margin.bottom = TEXTSPACING_UNINITIALIZED_VAL, \
    \
    .color = (SDL_Color){}, \
    .color_bg = (SDL_Color){}, \
    .lineskip = INVALID_VAL, \
    .wrap = INVALID_VAL, \
    .rotation = INVALID_VAL, \
    .flip = INVALID_VAL, \
    \
    .bx_width = INVALID_VAL, \
    .bx_height = INVALID_VAL, \
    \
    .scale_width = INVALID_VAL, \
    .scale_height = INVALID_VAL, \
    \
    .chr_dist = INVALID_VAL, \
    .chr_rotation = INVALID_VAL, \
    .chr_fmt = INVALID_VAL, \
    .chr_flip = INVALID_VAL, \
    \
    .x = INVALID_VAL, \
    .y = INVALID_VAL, \
    \
    .func_alterMargin = INVALID_PTR, \
    .func_alterBorder = INVALID_PTR, \
    .func_alterPadding = INVALID_PTR, \
    .func_alterText = INVALID_PTR, \
    \
    ##__VA_ARGS__ \
})

int TextConfig_Equals(TextConfig*, TextConfig*);
int TextConfig_TextuallyEquals(TextConfig*, TextConfig*);

/*
 * Preenche os campos "nao definidos" de a (INVALID_VAL, INVALID_PTR, cores
 * zeradas, lados de espacamento nao definidos) com os valores de b.
 */
void TextConfig_Populate(TextConfig*a, const TextConfig*b);

#endif
