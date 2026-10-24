#ifndef GTEXTCHAR_H
#define GTEXTCHAR_H

#include<stddef.h>
#include<SDL2/SDL_pixels.h>
#include"GTextCharFormat.h"

struct TextChar {
    int chr;
    int width;
    int height;
    //
    double box_width;
    double box_height;
    //
    double rotation;
    double rotation_cos;
    double rotation_sin;
    //
    TextCharFormat fmt;
    SDL_Color color;

    size_t chr_textIdx;
    size_t chr_lineIdx;
    size_t line_idx;

    double x, y;
};

#endif
