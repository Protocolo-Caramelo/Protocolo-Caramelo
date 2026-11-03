#ifndef GAME_GLOBALS_CACHE_H
#define GAME_GLOBALS_CACHE_H

#include<graphical.h>

typedef enum GameFont GameFont;
typedef enum GameFont_Config GameFont_Config;
enum GameFont {
    GFONT_REGULAR,
    GFONT_TITLE,
    GFONT_3D,
    GFONT_DIGITAL,

    GFONT_COUNT
};
enum GameFont_Config {
    GFONT_CONFIG_PATH,
    GFONT_CONFIG_PTSIZE,

    GFONT_CONFIG_COUNT
};

extern TextCache *cache[GFONT_COUNT];
#define GFONT_CONFIG \
    {                                                                                           \
        [GFONT_REGULAR] = {                                                                     \
            [GFONT_CONFIG_PATH]   = "assets/fonts/pixeloid-font/PixeloidMono-nAOpP.ttf",        \
            [GFONT_CONFIG_PTSIZE] = (void*)100,                                                 \
        }, \
        [GFONT_TITLE] = {                                                                       \
            [GFONT_CONFIG_PATH]   = "assets/fonts/Press_Start_2P/PressStart2P-Regular.ttf",     \
            [GFONT_CONFIG_PTSIZE] = (void*)100,                                                 \
        }, \
        [GFONT_3D] = {                                                                          \
            [GFONT_CONFIG_PATH]   = "assets/fonts/3d/ka1.ttf",                                  \
            [GFONT_CONFIG_PTSIZE] = (void*)100,                                                 \
        }, \
        [GFONT_DIGITAL] = {                                                                     \
            [GFONT_CONFIG_PATH]   = "assets/fonts/digital-numbers/DigitalNumbers-Regular.ttf",  \
            [GFONT_CONFIG_PTSIZE] = (void*)100,                                                 \
        },                                                                                      \
    }

void GameCache_Init();
void GameCache_Quit();

#endif
