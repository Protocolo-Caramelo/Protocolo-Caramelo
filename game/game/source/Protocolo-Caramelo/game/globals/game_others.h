#ifndef GAME_GLOBALS_OTHERS_H
#define GAME_GLOBALS_OTHERS_H

extern int w_width;
extern int w_height;
extern double w_ratio;
#define WINDOW_RATIO_W (ELEMENT_WINDOW_W / (double)ELEMENT_WINDOW_H)
#define WINDOW_RATIO_H (ELEMENT_WINDOW_H / (double)ELEMENT_WINDOW_W)

#define SWAP_VALUES(a, b) \
    do {            \
        a += b;     \
        b = a - b;  \
        a -= b;      \
    }while(0)

#endif
