#include"game.h"
#include"game_globals.h"

// Game methods
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

void Game_PlayInit()
{
    game_state = GAME_STATE_SCREEN_PLAY;
    game_playState = GAME_PLAY_STATE_VIEW_ROOM_1;
    gamePlay_tex = GPLAY_TEX_ROOM_1;
}

void Game_PlayQuit()
{
    Game_Quit();
}

// Other usefull functions
double Game_GetScaleW()
{
    int width, height;
    SDL_GetWindowSize(w, &width, &height);
    return width / (double)GCORE_WINDOW_W;
}

double Game_GetScaleH()
{
    int width, height;
    SDL_GetWindowSize(w, &width, &height);
    return height / (double)GCORE_WINDOW_H;
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
            .y = w_height - (*Text_edges(fps))[DRAW_HEIGHT],
        ),
        NULL
    );

    fps_time = SDL_GetTicks();

#endif
}
