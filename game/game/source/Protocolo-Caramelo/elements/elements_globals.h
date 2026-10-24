#ifndef ELEMENTS_GLOBALS_H
#define ELEMENTS_GLOBALS_H

#include<graphical.h>

#define ELEMENT_WINDOW_NAME "Protocolo-Caramelo"
#define ELEMENT_WINDOW_X_POSITION SDL_WINDOWPOS_UNDEFINED
#define ELEMENT_WINDOW_Y_POSITION SDL_WINDOWPOS_UNDEFINED
#define ELEMENT_WINDOW_W 960
#define ELEMENT_WINDOW_H 600
#define ELEMENT_WINDOW_FLAGS (SDL_WINDOW_RESIZABLE)
extern SDL_Window *w;
//
#define ELEMENT_RENDERER_FLAGS (SDL_RENDERER_ACCELERATED)
extern SDL_Renderer *r;
//
extern SDL_Event e;

#define ELEMENT_CACHE_FREGULAR_PATH "assets/fonts/pixeloid-font/PixeloidMono-nAOpP.ttf"
#define ELEMENT_CACHE_FREGULAR_PTSIZE 100
extern TextCache *cache_fRgular;
#define ELEMENT_CACHE_FTITLE_PATH "assets/fonts/Press_Start_2P/PressStart2P-Regular.ttf"
#define ELEMENT_CACHE_FTITLE_PTSIZE 100
extern TextCache *cache_fTitle;

typedef enum GameState GameState;
enum GameState {
    GAME_STATE_SCREEN_MENU,
    GAME_STATE_SCREEN_CONFIG,
    // GAME_STATE_SCREEN_ABOUT,
    GAME_STATE_PLAYING,
    GAME_STATE_PLAYING_MONITOR
};

#endif
