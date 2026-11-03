#include"miner.h"

void AppMiner_Init(void)
{
}

void AppMiner_Quit(void)
{
}

void AppMiner_Draw(const SDL_Rect *body)
{
    printf("%i %i %i %i\n", body->x, body->y, body->w, body->h);
}

void AppMiner_HandleEvent(const SDL_Event *e, const SDL_Rect *body, int pointer_inside)
{
}
