#include"game_core.h"

SDL_Window *w;
SDL_Renderer *r;
SDL_Event e;

void GameCore_Init()
{

    w = SDL_CreateWindow(
        GCORE_WINDOW_NAME,
        GCORE_WINDOW_X_POSITION,
        GCORE_WINDOW_Y_POSITION,
        GCORE_WINDOW_W,
        GCORE_WINDOW_H,
        GCORE_WINDOW_FLAGS
    );
    r = SDL_CreateRenderer(
        w,
        -1,
        GCORE_RENDERER_FLAGS
    );
}

void GameCore_Quit()
{
    SDL_DestroyRenderer(r);
    SDL_DestroyWindow(w);
}
