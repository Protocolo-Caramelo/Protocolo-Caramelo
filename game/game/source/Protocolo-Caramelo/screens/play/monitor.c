#include"monitor.h"

static SDL_Rect m_texDest;
static SDL_Rect m_texDestRaw;

void Monitor_handleInput(SDL_Event*e)
{
    switch(e->type){
        case SDL_QUIT:
            Game_PlayQuit();
            break;

        default:
            break;
    }
}

void Monitor_Render()
{
    SDL_SetRenderDrawColor(r, 255, 0, 0, 255);
    SDL_RenderClear(r);

    while(SDL_PollEvent(&e)){
        Monitor_handleInput(&e);
    }

    Game_PlayDisplayBackground(GAMEPLAY_TEX_MONITOR, &m_texDest, &m_texDestRaw);
}
