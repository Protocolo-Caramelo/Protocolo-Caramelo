#include"GTexture.h"

void Texture_Clear(Texture*t)
{
    SDL_Renderer *renderer;
    SDL_Texture *target_prev;
    Uint8 r, g, b, a;

    renderer = *Texture_renderer(t);
    if(!renderer || !*Texture_raw(t)){
        return;
    }

    target_prev = SDL_GetRenderTarget(renderer);
    SDL_GetRenderDrawColor(renderer, &r, &g, &b, &a);

    SDL_SetRenderTarget(renderer, *Texture_raw(t));
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
    SDL_RenderClear(renderer);

    SDL_SetRenderTarget(renderer, target_prev);
    SDL_SetRenderDrawColor(renderer, r, g, b, a);
}
