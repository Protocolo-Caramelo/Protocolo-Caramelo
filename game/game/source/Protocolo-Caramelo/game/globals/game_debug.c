#include"game_debug.h"
#include"game_cache.h"

Text *fps;
int fps_time;
int fps_delayTime;
const int fps_delay = 100;
char fps_buffer[32];

void GameDebug_Init()
{
#ifdef GAME_DEBUG
    fps = Text_Create(
        cache[GFONT_REGULAR],
        FPS_CONFIG
    );
    fps_time = fps_delayTime = SDL_GetTicks();
#endif
}

void GameDebug_Quit()
{
    Text_Destroy(&fps);
}
