#ifndef GTEXTSPACING_H
#define GTEXTSPACING_H

#include"GCommons.h"
#include"GTextTypedefs.h"
#include<SDL2/SDL_pixels.h>
#include<SDL2/SDL_render.h>
#include<limits.h>

/*
 * TextSpacing Struct
 *
 * Usada por Padding, Border e Margin. Um lado com o valor
 * TEXTSPACING_UNINITIALIZED_VAL significa "nao definido" (herda do Text);
 * uma cor totalmente zerada (0, 0, 0, 0) tambem significa "nao definida".
 */
#define TEXTSPACING_UNINITIALIZED_VAL INT_MAX
struct TextSpacing {
    unsigned
        top,
        right,
        bottom,
        left;

    SDL_Color color;
};

//
// Methods
//
void TextSpacing_Populate(TextSpacing*a, const TextSpacing*b);
int TextSpacing_Equals(const TextSpacing*a, const TextSpacing*b);
/* Retorna 1 se os quatro lados estao "nao definidos". */
int TextSpacing_IsUninitialized(const TextSpacing*a);

/* Nome antigo (enganoso: nao testa zero), mantido por compatibilidade. */
#define TextSpacing_Zered TextSpacing_IsUninitialized

#endif
