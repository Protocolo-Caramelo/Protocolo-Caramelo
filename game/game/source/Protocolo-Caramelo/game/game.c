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
    gameState = GAME_STATE_SCREEN_PLAY;
    gamePlay_state = GAMEPLAY_STATE_VIEW_ROOM_1;
    gamePlay_tex = GAMEPLAY_TEX_ROOM_1;
}

void Game_PlayQuit()
{
    Game_Quit();
}

void Game_PlayDisplayBackground(GamePlay_Texture tex, SDL_Rect *tex_rect, SDL_Rect *tex_rectRaw)
{
    SDL_Texture *t;
    int tex_w, tex_h;
    int tex_nW, tex_nH;

    t = play_tex[tex];
    SDL_GetWindowSize(w, &w_width, &w_height);
    w_ratio = w_width / (double)w_height;
    SDL_QueryTexture(
        t,
        NULL, NULL,
        &tex_w, &tex_h
    );

    if(w_width / (double)w_height < tex_w / (double)tex_h){
        tex_nW = w_width;
        tex_nH = w_width * (ELEMENT_WINDOW_H / (double)ELEMENT_WINDOW_W);
    }
    else {
        tex_nH = w_height;
        tex_nW = w_height * (ELEMENT_WINDOW_W / (double)ELEMENT_WINDOW_H);
    }
    *tex_rect = (SDL_Rect){
        .x = w_width / 2 - tex_nW / 2,
        .y = w_height / 2 - tex_nH / 2,
        .w = tex_nW,
        .h = tex_nH
    };
    *tex_rectRaw = (SDL_Rect){
        .x = w_width / 2 - tex_nW / 2,
        .y = w_height / 2 - tex_nH / 2,
        .w = tex_w,
        .h = tex_h
    };

    SDL_RenderCopy(
        r,
        t,
        NULL,
        tex_rect
    );
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
