#ifndef GAME_GLOBALS_DEBUG
#define GAME_GLOBALS_DEBUG

#if defined(COMPILATION_DEVQUOTE) || defined(COMPILATION_TOOLCHAIN_WINDOWS_DEVQUOTE)
#define GAME_DEBUG
#endif

#ifdef GAME_DEBUG
#include<graphical.h>

extern Text *fps;
extern int fps_time;
extern int fps_delayTime;
extern const int fps_delay;
extern char fps_buffer[32];
#define FPS_CONFIG              \
    TextConfig_Create(          \
        .scale_width = 1 / 4.0, \
        .scale_height = 1 / 4.0,\
        .color = (SDL_Color){   \
            .r = 255,           \
            .g = 255,           \
            .a = 255            \
        },                      \
        .color_bg = (SDL_Color){\
            .b = 255,           \
            .a = 255            \
        }                       \
    )
#endif

void GameDebug_Init();
void GameDebug_Quit();

#endif
