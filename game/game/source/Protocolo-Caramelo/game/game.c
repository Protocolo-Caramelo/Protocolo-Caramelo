#include"game.h"

//
void Game_Init()
{
    graphical_init();
    game_globals_init();
}

void Game_Quit()
{
    game_globals_quit();
    graphical_quit();
    SDL_Quit();
    exit(0);
}

//
void Game_PlayInit()
{
    gameState = GAME_STATE_PLAYING_VIEW_ROOM_1;
    //
}

void Game_PlayQuit()
{
}

//
void Game_Debug()
{
#ifdef GAME_DEBUG
    SDL_GetWindowSize(
        w,
        &w_width, &w_height
    );

    if(SDL_GetTicks() - fps_delayTime >= fps_delay){
        sprintf(fps_buffer, "FPS: %.2lf", 1000.0 / (SDL_GetTicks() - fps_time));
        fps_delayTime = SDL_GetTicks();
    }
    Text_DrawUTF8(
        fps,
        fps_buffer,
        TextConfig_Create(
            .x = w_width - (*Text_edges(fps))[DRAW_WIDTH],
            .y = 0,
        ),
        NULL
    );

    fps_time = SDL_GetTicks();

#endif
}
