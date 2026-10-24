#ifndef GTEXTCHARRECT_H
#define GTEXTCHARRECT_H

#include<SDL2/SDL_pixels.h>
#include"GTextTypedefs.h"
#include"GTextCacheGlyph.h"

struct TextCharRect {
    Glyph *src; // from cache (NULL: caractere sem desenho, ex.: '\n')
    int x, y;
    int width, height;
    double rotation;
    SDL_Color color;
};

#endif
